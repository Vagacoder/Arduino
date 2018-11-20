/*
  CS137
  Calculator
  Qirui Hu

  1. Project summary
  The idea is to make a simple calculator, which can work with integer and float numbers.
  Arithmetic operations include addition (+), subtraction (-), multiplication (*) and division (/).
  However, this desing require at least 16 buttons (0-9, +, -, *, /, .(decimal point) and = )
  from 4 sparkfun kits. Therefore, I decide to use 2 soft potentiometers, each can be divided 
  to 5 regions as 5 buttons. In addion, I use one push button for each potentiometer, to switch 
  input state (like Shift key on keyboard), so that total 20 buttons (or keys), 2 * 5 * 2, are available.

  This design is a little complecated, and is not user friendly. To imporve user interface, I decide to use
  a LCD to display the state of potentiometer, input data and result.
  
  2. Components used:
  LCD x1: for display
  Soft potentiometer x2: for input
  Push button x2: for input, switch the state of soft potentiometer
  Potentiometer x1: for adjust the brightness of LCD
  
  3. Features:
  3.1. This calculator can do integer and float arithmetic operations: addition(+), subtraction (-), 
  multiplication (*) and division (/).
  3.2. Each soft potentiometer has two input state U(p) and D(own), the input state is switched by
  corresponding push buttons.
  3.3 LCD is used to visualize the state of potentiometer, input data and result:
    Soft potentiometer state is displayed on top line of LCD. For example, 
    "1: U  2: D", indicating soft potentiometer #1 is at U and #2 is at D.
    Input expression is displayed on left side of second line, result is displayed on the right
    side of second line. For example, "14.3+7.6=   21.9"

  4. Key mapping
  4.1. Soft potentiometer #1 key mapping
     +---+---+---+---+---+
   U | 0 | 1 | 2 | 3 | 4 |
     +---+---+---+---+---+
   D | 5 | 6 | 7 | 8 | 9 |
     +---+---+---+---+---+
  4.2. Soft potentiometer #2 key mapping
     +---+---+---+---+---+
   U | + | - | * | / | . |
     +---+---+---+---+---+
   D | = |     reset     |
     +---+---------------+

*/

// load the LCD library and setup pins
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// define constants
// define GPIO pins
// 1. push buttons
const int buttonPin1 = 7;  // pushbutton 1 pin
const int buttonPin2 = 8;  // pushbutton 2 pin

// 2. soft potentiometer
const int sensorPin1 = 0;      // Analog input pin
const int sensorPin2 = 1;      // Analog input pin

// setup other variables
// a boolean var for potentiometer reading state, 0: not in reading state, 1: in reading state
int inputReading = 1;
// a boolean var for push button reading state, 0: not in reading state, 1: in reading state
int buttonReading = 1;
// potentiometer state, 0: U(p), 1: D(own)
int input1State = 0;
int input2State = 0;

// define a union to hold the calculation result of input expression
// the calculation result could be an integer or a float.
union result {
  int intResult;
  float ftResult;
} fResult = {0};

// define a struction to hold input expression and calculation result
// expression length is limited to 10 char.
struct calculation {
  char expression[10];
  union result* finalResult;
} calculation = {{'\0'}, &fResult};


void setup() {
  // Set up the pushbutton pins to be an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  // setup serial output for screening
  Serial.begin(9600);

  // setup LCD, display mode is 2 lines of 16 characters
  lcd.begin(16, 2);
  lcd.clear();
  lcd.cursor();
  
  // print LCD first line which display the states of 2 soft potentiometers.
  lcd.setCursor(0, 0);
  lcd.print("1: U");    // U/D state of soft potentiometer #1 
  lcd.setCursor(7, 0);
  lcd.print("2: U");    // U/D state of soft potentiometer #2
  lcd.setCursor(0, 1);
  lcd.print("                "); // clean LCD second line
}

void loop() {

  // read from 2 push buttons
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);

  // ensure read only once for each pushing on buttons
  // and update the input states (0: U or 1: D) of 2 soft potentiometers
  if (buttonReading) {
    if (buttonState1 == LOW || buttonState2 == LOW) {
      buttonReading = 0;
      if (buttonState1 == LOW) {
        input1State = (input1State + 1) % 2;
      } else if (buttonState2 == LOW) {
        input2State = (input2State + 1) % 2;
      }
    }

    // display the input states (0: U or 1: D) of 2 soft potentiometers on LCD first line
    if (input1State == 0) {
      lcd.setCursor(0, 0);
      lcd.print("1: U");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("1: D");
    }

    if (input2State == 0) {
      lcd.setCursor(7, 0);
      lcd.print("2: U");
    } else {
      lcd.setCursor(7, 0);
      lcd.print("2: D");
    }

  } else if (!buttonReading && buttonState1 == HIGH && buttonState2 == HIGH) {
    buttonReading = 1;
  }

  // read from 2 potentiometers and output to serial for screening
  int sensorValue1 = analogRead(sensorPin1);
  int sensorValue2 = analogRead(sensorPin2);

  // output information to serial for debug
  Serial.print("Sensor 1 read is ");
  Serial.println(sensorValue1);
  Serial.print("Sensor 2 read is ");
  Serial.println(sensorValue2);
  Serial.print("input 1 State read is ");
  Serial.println(input1State);
  Serial.print("input 2 State read is ");
  Serial.println(input2State);

  // length of expression stored in structure calculation
  int expressionLength = strlen(calculation.expression);
  
  // a string to store input char
  char currentInput = '\0';
  
  // ensure reading only once for each pushing on soft potentiometer
  // the base line of soft potentiometer is 95, any read below 95 is considered as no input
  if (inputReading && (sensorValue1 > 95 || sensorValue2 > 95)) {
    // once get a reading, set inputReading = 0, no more reading before releasing the soft potentiometer
    inputReading = 0;

    // reading from potentiometer #1
    if (sensorValue1 > 95) {

      // state of potentiometer #1 is U
      if (input1State == 0) {
        if (sensorValue1 > 95 && sensorValue1 <= 280) {
          currentInput = '0';
        } else if (sensorValue1 > 280 && sensorValue1 <= 470) {
          currentInput = '1';
        } else if (sensorValue1 > 470 && sensorValue1 <= 660) {
          currentInput = '2';
        } else if (sensorValue1 > 660 && sensorValue1 <= 850) {
          currentInput = '3';
        } else if (sensorValue1 > 850 && sensorValue1 <= 1023) {
          currentInput = '4';
        }

      // append input char to expression stored in structure calculation
        if (expressionLength < 10) {
          calculation.expression[expressionLength] = currentInput;
        }

      } else {  // state of potentiometer #1 is D
        if (sensorValue1 > 95 && sensorValue1 <= 280) {
          currentInput = '5';
        } else if (sensorValue1 > 280 && sensorValue1 <= 470) {
          currentInput = '6';
        } else if (sensorValue1 > 470 && sensorValue1 <= 660) {
          currentInput = '7';
        } else if (sensorValue1 > 660 && sensorValue1 <= 850) {
          currentInput = '8';
        } else if (sensorValue1 > 850 && sensorValue1 <= 1023) {
          currentInput = '9';
        }

        // append input char to expression stored in structure calculation
        if (expressionLength < 10) {
          calculation.expression[expressionLength] = currentInput;
        }
      }

    // reading from potentiometer #2
    } else if (sensorValue2 > 95) {
      // state of potentiometer #2 is U
      if (input2State == 0) {
        if (sensorValue2 > 95 && sensorValue2 <= 280) {
          currentInput = '+';
        } else if (sensorValue2 > 280 && sensorValue2 <= 470) {
          currentInput = '-';
        } else if (sensorValue2 > 470 && sensorValue2 <= 660) {
          currentInput = '*';
        } else if (sensorValue2 > 660 && sensorValue2 <= 850) {
          currentInput = '/';
        } else if (sensorValue2 > 850 && sensorValue2 <= 1023) {
          currentInput = '.';
        }
        
        // append input char to expression stored in structure calculation
        if (expressionLength < 10) {
          calculation.expression[expressionLength] = currentInput;
        }

      } else { // state of potentiometer #2 is D
        if (sensorValue2 > 95 && sensorValue2 <= 280) {
          currentInput = '=';

          // append input char to expression stored in structure calculation
          if (expressionLength < 10) {
            calculation.expression[expressionLength] = currentInput;
          }
          
          // and call the function of parseExpression()
          // return the resultType, 0: integer, 1: float
          int resultType = parseExpression();

          // print the calculated result on LCD second line, starting at the position of 11
          lcd.setCursor(11, 1);
          // print integer or float depending on result type, 0: integer, 1: float
          if (resultType == 0) {
            lcd.print(calculation.finalResult->intResult);
          } else {
            lcd.print(calculation.finalResult->ftResult);
          }

        // push reset button
        } else if (sensorValue1 > 280 && sensorValue1 <= 1023) {
          reset();
        }
      }
    }

    // print the expression on LCD second line, starting at the position of 0
    lcd.setCursor(0, 1);
    lcd.print(calculation.expression);

  // if soft potentiometers are not in reading state, no pushing on soft potentiometer,
  // set soft potentiometers to reading state 
  } else if (!inputReading && sensorValue1 <= 95 && sensorValue2 <= 95) {
    inputReading = 1;
  }

}

/*
  The parseExpression take a string, calculation.expression, and parse it to arithmetic expression,
  calculate result, then store the result to calculation.finalResult.
  The finalResult is a pointer to a union, which can store int or float.
  return 0 if the result is integer; 1 if the result is float
*/
int parseExpression() {
  int resultType = 0;

  // a pointer to arithmetic operator(+, -, * or /) in expression 
  char* arithmetic;

  // find the position of arithmetic operator, assign it to char* arithmetic
  for (int i = 0; i < strlen(calculation.expression); ++i) {
    char* cursor1 = &(calculation.expression[i]);
    if (*cursor1 == '+' || *cursor1 == '-' || *cursor1 == '*' || *cursor1 == '/') {
      arithmetic = cursor1;
      break;
    }
  }

  // check whether expression is float or integer by checking the presence of decimal point '.' in expression
  // if there is a decimal point, calculate as float numbers
  if (strchr(calculation.expression, '.') != NULL) {
    resultType = 1;
    float number1 = atof(calculation.expression);
    float number2 = atof(arithmetic + 1);

    if (*arithmetic == '+') {
      calculation.finalResult->ftResult = number1 + number2;
    } else if (*arithmetic == '-') {
      calculation.finalResult->ftResult = number1 - number2;
    } else if (*arithmetic == '*') {
      calculation.finalResult->ftResult = number1 * number2;
    } else if (*arithmetic == '/') {
      calculation.finalResult->ftResult = number1 / number2;
    }

  // if there is no decimal point, calculate as integer numbers
  } else {
    int number1 = atoi(calculation.expression);
    int number2 = atoi(arithmetic + 1);

    if (*arithmetic == '+') {
      calculation.finalResult->intResult = number1 + number2;
    } else if (*arithmetic == '-') {
      calculation.finalResult->intResult = number1 - number2;
    } else if (*arithmetic == '*') {
      calculation.finalResult->intResult = number1 * number2;
    } else if (*arithmetic == '/') {
      calculation.finalResult->intResult = number1 / number2;
    }
  }

  return resultType;
}

/*
 * function to reset data in structure calculation, and clean the LCD second line
 */
void reset() {
  // reset data in structure calculation
  calculation.expression[0] = '\0';
  calculation.finalResult->intResult = 0;

  // clean the LCD second line
  lcd.setCursor(0, 1);
  lcd.print("                ");

  // output for debug
  Serial.println("  RESET  ");
}

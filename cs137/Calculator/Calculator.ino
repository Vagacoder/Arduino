/*
  Qirui Hu
  CS137
  Calculator
  
  1. Components:
  LCD x1: for display
  Soft potentiometer x2: for input
  Push button x2: for input, switch the state of soft potentiometer
  Potentiometer x1: for adjust the brightness of LCD
  
  2. Features:
  2.1. This calculator can do integer and float arithmetic operations, including addition(+),
  subtraction (-), multiplication (*) and division (/).
  2.2. Each soft potentiometer has two input state U(p) and D(own), the state is switched by
  corresponding push buttons.

  Soft potentiometer #1 key mapping
     =====================
   U | 0 | 1 | 2 | 3 | 4 |
   D | 5 | 6 | 7 | 8 | 9 |
   
  Soft potentiometer #2 key mapping
     =====================
   U | + | - | * | / | . |
   D | = |     clear     |
   
  2.3. Soft potentiometer state is displayed on top line of LCD, like "1: U  2: D", suggesting 
  soft potentiometer is at U and the other is at D.
 
*/

// load the LCD library and setup pins
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// define constants
// define GPIO pins
enum pins {
  sensorPin1 = 0, sensorPin2, buttonPin1 = 7, buttonPin2 
}
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
// potentiometer state, 0: U, 1: D
int input1State = 0;
int input2State = 0;

union result {
  int intResult;
  float ftResult;
} fResult = {0};

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

  // setup LCD 2 lines of 16 characters
  lcd.begin(16, 2);
  lcd.clear();
  lcd.cursor();
  lcd.setCursor(0, 0);
  lcd.print("1: U");    // U/D state of potentiometer #1 at line 1, positision 3
  lcd.setCursor(7, 0);
  lcd.print("2: U");    // U/D state of potentiometer #2 at line 1, positision 10
  lcd.setCursor(0, 1);
  lcd.print("                ");

}

void loop() {

  // read from 2 push buttons
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);

  if (buttonReading) {
    if (buttonState1 == LOW || buttonState2 == LOW) {
      buttonReading = 0;
      if (buttonState1 == LOW) {
        input1State = (input1State + 1) % 2;
      } else if (buttonState2 == LOW) {
        input2State = (input2State + 1) % 2;
      }
    }

    // display potentiometers state U or D on LCD
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
  Serial.print("Sensor 1 read is ");
  Serial.println(sensorValue1);
  Serial.print("Sensor 2 read is ");
  Serial.println(sensorValue2);
  Serial.print("input 1 State read is ");
  Serial.println(input1State);
  Serial.print("input 2 State read is ");
  Serial.println(input2State);

  int expressionLength = strlen(calculation.expression);
  char currentInput = '\0';
  //
  if (inputReading && (sensorValue1 > 95 || sensorValue2 > 95)) {
    // once get a reading, set inputReading = 0, no more reading in this loop
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

        if (expressionLength < 10) {
          calculation.expression[expressionLength] = currentInput;
        }
      }

      // reading from potentiometer #2
    } else if (sensorValue2 > 95) {
      // state of potentiometer #1 is U
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

        if (expressionLength < 10) {
          calculation.expression[expressionLength] = currentInput;
        }

      } else { // state of potentiometer #2 is D
        if (sensorValue2 > 95 && sensorValue2 <= 280) {
          currentInput = '=';

          if (expressionLength < 10) {
            calculation.expression[expressionLength] = currentInput;
          }

          int resultType = parseExpression();
          lcd.setCursor(11, 1);
          if (resultType == 0) {
            lcd.print(calculation.finalResult->intResult);
          } else {
            lcd.print(calculation.finalResult->ftResult);
          }

        } else if (sensorValue1 > 280 && sensorValue1 <= 1023) {
          reset();
        }


      }
    }
    lcd.setCursor(0, 1);
    lcd.print(calculation.expression);

  } else if (!inputReading && sensorValue1 <= 95 && sensorValue2 <= 95) {
    inputReading = 1;
  }



}

/*
   return 0 if it is integer; 1 if it is float
*/
int parseExpression() {
  int resultType = 0;
  char* arithmetic;

  for (int i = 0; i < strlen(calculation.expression); ++i) {
    char* cursor1 = &(calculation.expression[i]);
    if (*cursor1 == '+' || *cursor1 == '-' || *cursor1 == '*' || *cursor1 == '/') {
      arithmetic = cursor1;
      break;
    }
  }

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

void reset() {
  calculation.expression[0] = '\0';
  lcd.setCursor(0, 1);
  lcd.print("                ");
  Serial.println("  RESET  ");
}


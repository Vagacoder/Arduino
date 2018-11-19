/*
  Qirui Hu
  CS137
  Calculator
*/

// load the LCD library and setup pins
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// define constants
// 1. push buttons
const int buttonPin1 = 7;  // pushbutton 1 pin
const int buttonPin2 = 8;  // pushbutton 2 pin

// 2. soft potentiometer
const int sensorPin1 = 0;      // Analog input pin
const int sensorPin2 = 1;      // Analog input pin

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
  lcd.setCursor(0, 1);
  lcd.print("1: U");    // U/D state of potentiometer #1 at line 1, positision 3
  lcd.setCursor(7, 1);
  lcd.print("2: U");    // U/D state of potentiometer #2 at line 1, positision 10
  lcd.setCursor(0, 2);

  // setup other variables
  // a boolean var for potentiometer reading state, 0: not in reading state, 1: in reading state
  int inputReading = 1;
  // a boolean var for push button reading state, 0: not in reading state, 1: in reading state
  int buttonReading = 1;
  // potentiometer state, 0: U, 1: D
  int input1State = 0;
  int input2State = 0;
}

void loop() {

  // read from 2 push buttons
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);

  if (buttonReading){
    if (buttonState1 == LOW || buttonState2 == LOW){
      buttonReading = 0;
    }
  } else if (!buttonReading && buttonState1 == HIGH && buttonState2 == HIGH){
    buttonReading = 1;
  }

  // read from 2 potentiometers and output to serial for screening
  int sensorValue1 = analogRead(sensorPin1);
  int sensorValue2 = analogRead(sensorPin2);
  Serial.print("Sensor 1 read is ");
  Serial.println(sensorValue1);
  Serial.print("Sensor 2 read is ");
  Serial.println(sensorValue2);



  //
  if (inputReading && (sensorValue1 > 10 || sensorValue2 > 10)) {
    inputReading = 0;

  } else if (!inputReading && sensorValue1 <= 10 && sensorValue2 <= 10) {
    inputReading = 1;
  }


}

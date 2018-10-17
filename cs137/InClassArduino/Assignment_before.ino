/*
In this assignment, you are given code to make 1 LED blink,
modify the code to make 5 leds blink 1 at a time.
Your code should:
(1) keep track of the list of ledpins in an array
(2) change the setup function to include setting up all LEDpins and use a loop (no repeated code)
(2) implement the function OneAtATime() to turn on/off all the LEDs 1 at a time
(3) The function you created to accomplish step (2) above should use a loop (no repeated code)
*/
 
/*************************************************************
 * Define Constants and Globals
 **************************************************************/
//TODO: change the variable ledPin to an int array ledPins to include 5 leds (pins 2 through 6)
const int numberOfLed = 5;
const int ledPins[] = {2, 3, 4, 5, 6};

/****************************************************************
 * Function Declarations
 ****************************************************************/
/* oneOnAtATime()
 *  This function will step through the LEDs,
 *  lighting only one at at time.
*/
void oneOnAtATime();

/*
  Hardware connections:
  You'll need 5 LEDs, and 5 330 Ohm resistors(orange-orange-brown).
  - For each LED, connect the negative side (shorter leg) to a 330 Ohm resistor.
  - Connect the other side of the resistors to GND.
  - Connect the positive side (longer leg) of the LEDs to Arduino digital pins 2 through 6.
*/

/**************************************************************
 * The setup() function runs once when the sketch starts.
*  You'll use it for things you need to do first, or only once:
 **************************************************************/ 
void setup()
{
  //TODO: include setting up all LEDpins and use a loop (no repeated code)
  for (int i = 0; i < numberOfLed; ++i){
    pinMode(ledPins[i],OUTPUT); //setup the ledPin to be an OUTPUT pin
  }
}

 /*******************************************************************
  * After setup() finishes, the loop() function runs over and over
  * again, forever (or until you turn off or reset the Arduino). 
  * This is usually where the bulk of your program lives:
 *************************************************************************/
void loop()
{
  //TODO: replace the code below to call your function oneOnAtATime()
//  int delayTime = 1000;
//  digitalWrite(ledPin, HIGH);  // turn LED on
//  delay(delayTime);                    // pause to slow down
//  digitalWrite(ledPin, LOW);   // turn LED off
//  delay(delayTime);                    // pause to slow down
  oneOnAtATime();

}

/****************************************************************
 * Function Definitions
 ****************************************************************/
/*
oneOnAtATime()
This function will step through the LEDs,
lighting only one at at time.
*/
void oneOnAtATime()
{
  //TODO: implement the function oneOnAtATime() to turn on/off all the LEDs 1 at a time

  for (int i = 0; i < numberOfLed; ++i){
  
    int delayTime = 20;
  digitalWrite(ledPins[i], HIGH);  // turn LED on
  delay(delayTime);                    // pause to slow down
  digitalWrite(ledPins[i], LOW);   // turn LED off
  delay(delayTime);                    // pause to slow down

  }
  
}

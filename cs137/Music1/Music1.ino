/*
  Play music (Edelweiss) with flashing lights.
  1, This program plays the music of Edelweiss,
  2, Seven LEDs will flash respectively according to the current note
  3, The 8th LED will flash for every note to show the beats of music
  4, Potentiometer #1 is used to adjust the playing speed of music
  5, Potentiometer #2 is used to adjust the volume of buzzer (only in hardware,
      no code used. Tried to use toneAC(), but did not work).
  6, PushBotton is used to start and stop the music playing.
      Note: pushbutton is sensitive to start, tapping it works well. However,
      pushbutton must be held to stop the music playing,.
  Qirui Hu
  Oct 2018
  CS137
*/

/*************************************************
   Public Constants for music notes (pitch to frequency)
 *************************************************/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// playing speed controled by potentiometer using Analog-In pin A0
int sensorPin = 0;

// LED light setting, using pin 2-10
const int LEDs[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

// buzzer setting, using pin 11
const int buzzerPin = 11;

// button setting, using pin 12
const int buttonPin = 12;

// Music settings
// 1. length of music
const int lengthOfEdelweiss = 55;
// 2. Notes (pitches) of edelweiss, see constans list above
const int edelweiss[] = { NOTE_E4, NOTE_G4, NOTE_D5, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4,
                          NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_D5, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_B4,
                          NOTE_C5, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_G4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_A4, NOTE_C5,
                          NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_D5, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_G4,
                          NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5
                        };
// 3. Beats of edelweiss (eighth note is 1; quarter note is 2; half note is 4; whole note is 8)
const int beatsOfEdelweiss[] = {4, 2, 8, 4, 2, 8, 4, 2, 2, 2, 2, 8, 8, 4, 2, 8, 4, 2, 8, 4, 2, 2, 2, 2, 8, 8, 4, 1, 1, 2, 2, 2,
                                4, 2, 8, 4, 2, 4, 2, 4, 2, 8, 4, 2, 8, 4, 2, 8, 4, 2, 2, 2, 2, 8, 8
                               };
// 4. basic tempo is set 150 ms
int tempo = 150;

// 5. set control vars for checking pushbutton
int playing = 0;
int buttonReady = 1;

void setup() {
  // turn on the buzzer output
  pinMode(buzzerPin, OUTPUT);

  // set LED lights to output
  // LED on pin10 will be on and off for every note
  // LEDs on 2-8 are corresponding to notes to Mi, Fa, Sol, La, Si,
  // high Do and high Re respectively.
  for (int i = 0; i < 9; ++i) {
    pinMode(LEDs[i], OUTPUT);
  }

  // Set up the pushbutton pin to input
  pinMode(buttonPin, INPUT);

  // Note:  **the analog input pin is always on, no need turn it on
}

void loop() {

  // check pushbutton (start or stop)
  int buttonState = digitalRead(buttonPin);
  if (buttonReady == 1) {
    if (buttonState == LOW && playing == 0) {
      playing = 1;
      buttonReady = 0;
    }
  }
  else {
    if (buttonState == HIGH) {
      buttonReady = 1;
    }
  }

  // play music
  if (playing) {
    playMusic();
    playing = 0;
    // make sure all lights are off after calling playMusic()
    for (int i = 0; i < 9; ++i) {
      digitalWrite(LEDs[i], LOW);
    }
  }
}

/*
   a function to play music
*/

void playMusic() {

  int i, duration;
  // assign the array of music notes to a pointer
  int length = lengthOfEdelweiss;
  int *music = edelweiss;
  int *beats = beatsOfEdelweiss;

  // for loop iterating the array of music
  for (i = 0; i < length; i++)  {

    // check pushbutton for stop
    int buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH && buttonReady == 0)    {
      buttonReady = 1;
    }
    if (buttonState == LOW && buttonReady == 1) {
      playing = 0;
      buttonReady = 0;
      return;
    }

    // LED on pin10 will be on and off for every note
    digitalWrite(10, HIGH);

    // calculation duration of notes based tempo
    duration = beats[i] * tempo;

    // play the current note
    if (music[i] != 0 && playing == 1) {
      tone(buzzerPin, music[i], duration);
    }

    // check pushbutton
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH && buttonReady == 0)    {
      buttonReady = 1;
    }
    if (buttonState == LOW && buttonReady == 1) {
      playing = 0;
      buttonReady = 0;
      return;
    }

    // call function of lightOn() to turn on one lights of LED 2-8
    // a pointer of current note is passed to turn the corresponding LED.
    if (music[i] != 0 && playing == 1) {
      lightOn(&music[i], duration);
    }
    // if lightOn function is interrupted by pushbutton, stop playing
    if (playing == 0) {
      return;
    }

    // check pushbutton
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH && buttonReady == 0)    {
      buttonReady = 1;
    }
    if (buttonState == LOW && buttonReady == 1) {
      playing = 0;
      buttonReady = 0;
      return;
    }

    // turn off all LED lights, so that they look like flashing
    for (int i = 0; i < 9; ++i) {
      digitalWrite(LEDs[i], LOW);
    }

    // check pushbutton
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH && buttonReady == 0)    {
      buttonReady = 1;
    }
    if (buttonState == LOW && buttonReady == 1) {
      playing = 0;
      buttonReady = 0;
      return;
    }

    // read the value from potentiometer for speeding
    int sensorValue = analogRead(sensorPin);
    // delay the value from potentiometer to adjust the playing speed.
    delay(sensorValue);
  }
}


/*
   a function to turn off LED corresponding to passed note
   @ptr: the pointer of current note in the array of music
   @duration: the duration of current note
*/
void lightOn(int* ptr, int duration) {
  if (playing == 0) {
    return;
  }

  // check pushbutton
  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && buttonReady == 0)    {
    buttonReady = 1;
  }
  if (buttonState == LOW && buttonReady == 1) {
    playing = 0;
    buttonReady = 0;
    return;
  }


  // dereference the pointer to note value
  int note = *ptr;
  // turn on corresponding LED
  switch (note) {
    case NOTE_E4 :
      digitalWrite(2, HIGH);
      break;
    case NOTE_F4:
      digitalWrite(3, HIGH);
      break;
    case NOTE_G4:
      digitalWrite(4, HIGH);
      break;
    case NOTE_A4:
      digitalWrite(5, HIGH);
      break;
    case NOTE_B4:
      digitalWrite(6, HIGH);
      break;
    case NOTE_C5:
      digitalWrite(7, HIGH);
      break;
    case NOTE_D5:
      digitalWrite(8, HIGH);
      break;
  }
  // turn on the LED for the time of duration
  // so that the light looks synchronizing with music playing
  delay(duration);
  delay(tempo / 5);
}


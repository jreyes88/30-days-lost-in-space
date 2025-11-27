#include "Arduino.h"
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

const byte ROW_PINS[ROWS] = { 5, 4, 3, 2 };
const byte COL_PINS[COLS] = { 6, 7, 8, 9 };

char BUTTONS[ROWS][COLS] = {
  { '1', '2', '3', 'A' },  // 1st row
  { '4', '5', '6', 'B' },  // 2nd row
  { '7', '8', '9', 'C' },  // 3rd row
  { '*', '0', '#', 'D' }   // 4th row
};

Keypad myAwesomePad = Keypad(makeKeymap(BUTTONS), ROW_PINS, COL_PINS, ROWS, COLS);

const unsigned int TONES[ROWS][COLS] = {
  { 31, 93, 147, 208 },
  { 247, 311, 370, 440 },
  { 523, 587, 698, 880 },
  { 1397, 2637, 3729, 0 }
};

const byte BUZZER_PIN = 10;

void setup() {
  Serial.begin(9600);
}


void loop() {
  char button_character = myAwesomePad.waitForKey();
  unsigned int tone_frequency = 0;
  for (byte i = 0; i < ROWS; i++) {
    for (byte j = 0; j < COLS; j++) {
      if (button_character == BUTTONS[i][j]) {
        tone_frequency = TONES[i][j];
      }

    }
  }

  Serial.print("Key: ");
  Serial.print(button_character);
  Serial.print("   Freq: ");
  Serial.println(tone_frequency);

  if (tone_frequency > 0) {                // If tone frequency greater than 0...
    tone(BUZZER_PIN, tone_frequency);  // ...then play the tone at that frequency until stopped
  } else {
    Serial.println("Stop tone");
    noTone(BUZZER_PIN);  // Stop pressed (tone frequency of 0) so stop any tone playing
  }
}
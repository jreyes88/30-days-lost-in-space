#include "Arduino.h"
#include <TM1637Display.h>

const byte CLK_PIN = 6;
const byte DIO_PIN = 5;

TM1637Display lander_display = TM1637Display(CLK_PIN, DIO_PIN);

const byte all_on[] = { 0b11111111,
                        0b11111111,
                        0b11111111,
                        0b11111111 };

const byte done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
  SEG_C | SEG_E | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
};

void setup() {
  lander_display.setBrightness(7);
}

void loop() {
  lander_display.clear();
  delay(1000);

  lander_display.setSegments(all_on);
  delay(1000);

  lander_display.clear();
  delay(1000);

  for (int i = 0; i < 4; i++) {
    lander_display.showNumberDecEx(1200, 0b01000000);
    delay(500);
    lander_display.clear();
    delay(500);
  }

  for (int i = -100; i <= 100; i++) {
    lander_display.showNumberDec(i);
    delay(50);
  }
  delay(1000);

  lander_display.clear();
  delay(1000);

  lander_display.setSegments(done);

  delay(10000);
}
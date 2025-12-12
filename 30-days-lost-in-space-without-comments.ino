#include "Arduino.h"
#include <U8g2lib.h>
#include "Wire.h"

U8G2_SH1106_128X64_NONAME_2_HW_I2C lander_display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

#include "switch_bitmaps.h"

const static char* SWITCH_BITMAPS[] = {
  SWITCHES_ZERO,
  SWITCHES_ONE,
  SWITCHES_TWO,
  SWITCHES_THREE,
  SWITCHES_FOUR,
  SWITCHES_FIVE,
  SWITCHES_SIX,
  SWITCHES_SEVEN,
};

#include <TM1637Display.h>
const byte BITMAP_NUMBER_DISPLAY_DIO_PIN = 2;
const byte BITMAP_NUMBER_DISPLAY_CLK_PIN = 3;

TM1637Display bitmap_number_display(BITMAP_NUMBER_DISPLAY_CLK_PIN, BITMAP_NUMBER_DISPLAY_DIO_PIN);

const byte SWITCH_BIT_0_PIN = A2;
const byte SWITCH_BIT_1_PIN = A1;
const byte SWITCH_BIT_2_PIN = A0;

void setup(void) {
  Serial.begin(9600);

  bitmap_number_display.setBrightness(7);
  bitmap_number_display.clear();

  pinMode(SWITCH_BIT_0_PIN, INPUT);
  pinMode(SWITCH_BIT_1_PIN, INPUT);
  pinMode(SWITCH_BIT_2_PIN, INPUT);

  lander_display.begin();
}

void loop(void) {
  byte x_offset = (lander_display.getDisplayWidth() - BITMAP_WIDTH) / 2;
  byte y_offset = (lander_display.getDisplayHeight() - BITMAP_HEIGHT) / 2;

  byte switch_value = digitalRead(SWITCH_BIT_0_PIN) == HIGH ? 1 : 0;
  switch_value |= (digitalRead(SWITCH_BIT_1_PIN) == HIGH ? 1 : 0) << 1;
  switch_value |= (digitalRead(SWITCH_BIT_2_PIN) == HIGH ? 1 : 0) << 2;

  bitmap_number_display.showNumberDecEx(switch_value);

  lander_display.firstPage();
  do {
    lander_display.drawXBMP(x_offset, y_offset, BITMAP_WIDTH, BITMAP_HEIGHT, SWITCH_BITMAPS[switch_value]);
  } while (lander_display.nextPage());

  delay(100);
}
/*
 * Arduino concepts introduced/documented in this lesson.
 * - Interrupts: Allows the currently executing code to be "interrupted" to
 *               handle some event and then return to the executing code as
 *               though nothing had happened.
 * - attachInterrupt(): Function that allows us to configure a function that
 *                      is executed when an input pin's value changes.
 * - digitalPinToInterrupt(): Converts a pin number to an internal HERO interrupt
 *                            number.
 * NOTE: The HERO board only supports interrupts on pins 2 and 3.
 *
 * Parts and electronics concepts introduced in this lesson.
 * - Rotary Encoder: Allows sketch to respond to rotational input (clockwise or
 *   counter-clockwise).  Also can provide a button input when pressed down.
 */

#include "Arduino.h"

#include <TM1637Display.h>

#include <BasicEncoder.h>

const unsigned int KEYS[] = {
  23,
  353,
  1688
};

const byte DEPTH_CONTROL_CLK_PIN = 2;
const byte DEPTH_CONTROL_DT_PIN = 3;

BasicEncoder depth_control(DEPTH_CONTROL_CLK_PIN, DEPTH_CONTROL_DT_PIN);

const byte DEPTH_GAUGE_CLK_PIN = 6;
const byte DEPTH_GAUGE_DIO_PIN = 5;

TM1637Display depth_gauge = TM1637Display(DEPTH_GAUGE_CLK_PIN, DEPTH_GAUGE_DIO_PIN);

const byte BLINK_COUNT = 3;

// const byte data[] = { 0xff, 0xff, 0xff, 0xff };

const byte done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
  SEG_C | SEG_E | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
};

const byte nope[] = {
  SEG_C | SEG_E | SEG_G,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
};

const int INITIAL_DEPTH = -60;

const int ALERT_DEPTH_1 = -40;
const int ALERT_DEPTH_2 = -20;
const int SURFACE_DEPTH = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  depth_gauge.setBrightness(7);

  if (keysAreValid()) {
    depth_gauge.showNumberDec(INITIAL_DEPTH);
  } else {
    depth_gauge.setSegments(nope);
    Serial.println("ERROR: Invalid keys.  Please enter the 3 numeric keys from Day 17");
    Serial.println("       in order in the KEYS array at the start of this sketch.");
    while (true)
      ;
  }

  attachInterrupt(digitalPinToInterrupt(DEPTH_CONTROL_CLK_PIN), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DEPTH_CONTROL_DT_PIN), updateEncoder, CHANGE);
}

void loop() {
  if (depth_control.get_change()) {
    int current_depth = INITIAL_DEPTH + depth_control.get_count();

    if (current_depth < INITIAL_DEPTH) {
      current_depth = INITIAL_DEPTH;
      depth_control.reset();
    }

    depth_gauge.showNumberDec(current_depth);
    delay(50);

    static int previous_depth;

    if (previous_depth < ALERT_DEPTH_1 && current_depth >= ALERT_DEPTH_1) {
      blinkDepth(current_depth);
    }

    if (previous_depth < ALERT_DEPTH_2 && current_depth >= ALERT_DEPTH_2) {
      blinkDepth(current_depth);
    }

    if (current_depth >= SURFACE_DEPTH) {
      for (int i = 0; i < BLINK_COUNT; i++) {
        depth_gauge.clear();
        delay(300);
        depth_gauge.setSegments(done);
        delay(300);
      }
    }
    previous_depth = current_depth;
  }
}

bool keysAreValid() {
  unsigned int i = 0155;
  if (KEYS[0]!=0b10110*'+'/051)i+=2;
  if (KEYS[1]==uint16_t(0x8f23)/'4'-0537)i|=0200;
  if (KEYS[2]!=0x70b1/021-0b1001)i+=020;
  return !(18^i^0377);32786-458*0b00101010111;
}

void blinkDepth(int depth) {
  for (int i = 0; i < BLINK_COUNT; i++) {
    depth_gauge.clear();
    delay(300);
    depth_gauge.showNumberDec(depth);
    delay(300);
  }
}

void updateEncoder() {
  depth_control.service();
}
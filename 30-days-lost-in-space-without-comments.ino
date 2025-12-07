#include "Arduino.h"
#include "Wire.h"
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C lander_display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void setup(void) {
  lander_display.begin();
}

void loop(void) {
  const byte TEST_PAGE_COUNT = 13;
  for (unsigned int display_frame = 0; display_frame < (TEST_PAGE_COUNT * 8); display_frame++) {
    lander_display.clearBuffer();

    lander_display.setFont(u8g_font_6x10);
    lander_display.setFontRefHeightExtendedText();
    lander_display.setFontPosTop();

    byte y_offset = 0;

    drawCenteredString(0, 0, "Exploration Lander");

    y_offset += lander_display.getMaxCharHeight();

    // const byte DISPLAY_PAGE = 12;
    // display_frame = (display_frame & 7) | (DISPLAY_PAGE << 3);

    switch (display_frame >> 3) {
      case 0: display_test_ready(y_offset, display_frame & 0b00000111); break;
      case 1: display_test_box_frame(y_offset, display_frame & 0b00000111); break;
      case 2: display_test_circles(y_offset, display_frame & 0b00000111); break;
      case 3: display_test_r_frame(y_offset, display_frame & 0b00000111); break;
      case 4: display_test_string(y_offset, display_frame & 0b00000111); break;
      case 5: display_test_line(y_offset, display_frame & 0b00000111); break;
      case 6: display_test_triangle(y_offset, display_frame & 0b00000111); break;
      case 7: display_test_ascii_1(y_offset); break;
      case 8: display_test_ascii_2(y_offset); break;
      case 9: display_test_extra_page(y_offset, display_frame & 0b00000111); break;
      case 10: display_test_bitmap_modes(y_offset, display_frame & 0b00000111, false); break;
      case 11: display_test_bitmap_modes(y_offset, display_frame & 0b00000111, true); break;
      case 12: display_test_bitmap_overlay(y_offset, display_frame & 0b00000111); break;
    }
    lander_display.sendBuffer();
    delay(100);
  }
}

byte drawCenteredString(byte x, byte y, char *string) {
  byte centered_x = x + ((lander_display.getDisplayWidth() - x) - lander_display.getStrWidth(string)) / 2;
  lander_display.drawStr(centered_x, y, string);
}

const byte LANDER_HEIGHT = 25;
const byte LANDER_WIDTH = 20;

void display_lander(byte x_location, byte y_location) {
  lander_display.drawFrame(x_location + 7, y_location, 6, 5);
  lander_display.drawFrame(x_location + 5, y_location + 4, 10, 20);
  lander_display.drawFrame(x_location, y_location + 6, 6, 16);
  lander_display.drawFrame(x_location + 14, y_location + 6, 6, 16);
  lander_display.drawTriangle(x_location + 2, y_location + 21,
                              x_location, y_location + 25,
                              x_location + 4, y_location + 25);
  lander_display.drawTriangle(x_location + 18, y_location + 21,
                              x_location + 15, y_location + 25,
                              x_location + 20, y_location + 25);
}

void display_test_ready(byte y_offset, byte frame) {
  const byte Y_CENTER = y_offset + ((lander_display.getDisplayHeight() - y_offset) / 2);
  const byte LANDER_PADDING = LANDER_WIDTH;
  const byte LANDER_Y_CENTER = Y_CENTER - (LANDER_HEIGHT / 2);
  display_lander(LANDER_PADDING, LANDER_Y_CENTER);

  if ((frame & 0b00000001) == 0) {
    const byte X_OFFSET = LANDER_PADDING + LANDER_WIDTH;
    byte text_height = lander_display.getMaxCharHeight();
    lander_display.setFontPosCenter();
    drawCenteredString(X_OFFSET, Y_CENTER - text_height, "Begin");
    drawCenteredString(X_OFFSET, Y_CENTER, "Hardware");
    drawCenteredString(X_OFFSET, Y_CENTER + text_height, "Test");
  }
}

const byte BOX1_WIDTH = 20;
const byte BOX1_HEIGHT = 10;
const byte BOX1_X_OFFSET = 5;
const byte BOX2_WIDTH = BOX1_WIDTH * 1.5;
const byte BOX2_HEIGHT = BOX1_HEIGHT * .8;
const byte BOX2_X_OFFSET = BOX1_X_OFFSET * 2;
const byte BOX2_Y_OFFSET = BOX1_HEIGHT / 2;

void display_test_box_frame(byte y_offset, byte frame) {
  drawCenteredString(0, y_offset, "drawBox");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.drawBox(BOX1_X_OFFSET, y_offset, BOX1_WIDTH, BOX1_HEIGHT);
  lander_display.drawBox(BOX2_X_OFFSET + frame, y_offset + BOX2_Y_OFFSET,
                         BOX2_WIDTH, BOX2_HEIGHT);

  y_offset += BOX2_Y_OFFSET + BOX2_HEIGHT;
  drawCenteredString(0, y_offset, "drawFrame");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.drawFrame(BOX1_X_OFFSET, y_offset, BOX1_WIDTH, BOX1_HEIGHT);
  lander_display.drawFrame(BOX2_X_OFFSET + frame, y_offset + BOX2_Y_OFFSET,
                           BOX2_WIDTH, BOX2_HEIGHT);
}

const byte CIRCLE1_RADIUS = 8;
const byte CIRCLE1_DIAMETER = (CIRCLE1_RADIUS * 2) + 1;
const byte CIRCLE1_X_OFFSET = CIRCLE1_RADIUS;
const byte CIRCLE2_RADIUS = CIRCLE1_RADIUS - 1;
const byte CIRCLE2_X_OFFSET = CIRCLE1_DIAMETER + CIRCLE2_RADIUS;

void display_test_circles(byte y_offset, byte frame) {
  drawCenteredString(0, y_offset, "drawDisc");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.drawDisc(CIRCLE1_X_OFFSET, y_offset + CIRCLE1_RADIUS, CIRCLE1_RADIUS);
  lander_display.drawDisc(CIRCLE2_X_OFFSET + frame, y_offset + CIRCLE2_RADIUS, CIRCLE2_RADIUS);

  y_offset += CIRCLE1_DIAMETER;
  drawCenteredString(0, y_offset, "drawCircle");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.drawCircle(CIRCLE1_X_OFFSET, y_offset + CIRCLE1_RADIUS, CIRCLE1_RADIUS);
  lander_display.drawCircle(CIRCLE2_X_OFFSET + frame, y_offset + CIRCLE2_RADIUS, CIRCLE2_RADIUS);
}

const byte RBOX1_WIDTH = 40;
const byte RBOX1_HEIGHT = 30;
const byte RBOX1_X_OFFSET = 5;
const byte RBOX2_WIDTH = 25;
const byte RBOX2_HEIGHT = 40;
const byte RBOX2_X_OFFSET = RBOX1_X_OFFSET + RBOX1_WIDTH + RBOX1_X_OFFSET;

void display_test_r_frame(byte y_offset, byte frame) {
  drawCenteredString(0, y_offset, "drawRFrame/Box");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.drawRFrame(RBOX1_X_OFFSET, y_offset, RBOX1_WIDTH, RBOX1_HEIGHT, frame + 1);
  lander_display.drawRBox(RBOX2_X_OFFSET, y_offset, RBOX2_WIDTH, RBOX2_HEIGHT, frame + 1);
}

const byte STRING_X_OFFSET = 30;

void display_test_string(byte y_offset, byte frame) {
  byte y_center = y_offset + (lander_display.getDisplayHeight() - y_offset) / 2;
  lander_display.setFontDirection(0);
  lander_display.drawStr(STRING_X_OFFSET + frame, y_center, " 0");
  lander_display.setFontDirection(1);
  lander_display.drawStr(STRING_X_OFFSET, y_center + frame, " 90");
  lander_display.setFontDirection(2);
  lander_display.drawStr(STRING_X_OFFSET - frame, y_center, " 180");
  lander_display.setFontDirection(3);
  lander_display.drawStr(STRING_X_OFFSET, y_center - frame, " 270");
  lander_display.setFontDirection(0);
}

const byte LINE_X_OFFSET = 7;
const byte LINE_Y_MAX = 55;

void display_test_line(byte y_offset, byte frame) {
  drawCenteredString(0, y_offset, "drawLine");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.drawLine(LINE_X_OFFSET + frame, y_offset,
                          40, LINE_Y_MAX);
  lander_display.drawLine(LINE_X_OFFSET + frame * 2, y_offset,
                          60, LINE_Y_MAX);
  lander_display.drawLine(LINE_X_OFFSET + frame * 3, y_offset,
                          80, LINE_Y_MAX);
  lander_display.drawLine(LINE_X_OFFSET + frame * 4, y_offset,
                          100, LINE_Y_MAX);
}

void display_test_triangle(byte y_offset, byte frame) {
  drawCenteredString(0, y_offset, "drawTriangle");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.drawTriangle(14, y_offset + 7,
                              45, y_offset + 20,
                              10, y_offset + 30);
  lander_display.drawTriangle(14 + frame, y_offset + 7 - frame,
                              45 + frame, y_offset + 20 - frame,
                              57 + frame, y_offset + 00 - frame);
  lander_display.drawTriangle(57 + frame * 2, y_offset + 0,
                              45 + frame * 2, y_offset + 20,
                              96 + frame * 2, y_offset + 43);
  lander_display.drawTriangle(10 + frame, y_offset + 30 + frame,
                              45 + frame, y_offset + 20 + frame,
                              96 + frame, y_offset + 43 + frame);
}

const byte FIRST_PRINTABLE_CHARACTER = 32;
void display_test_ascii_1(byte y_offset) {
  drawCenteredString(0, y_offset, "ASCII page 1");
  y_offset += lander_display.getMaxCharHeight();

  // For more characters visible you can uncomment the following line for a smaller font
  // lander_display.setFont(u8g2_font_spleen5x8_mf);
  byte character_width = lander_display.getMaxCharWidth();
  byte column_count = lander_display.getDisplayWidth() / character_width;
  byte row_count = (lander_display.getDisplayHeight() - y_offset) / lander_display.getMaxCharHeight();

  for (byte row = 0; row < row_count; row++) {
    for (byte column = 0; column < column_count; column++) {
      char character_string[2];
      byte character_number = FIRST_PRINTABLE_CHARACTER + (row * column_count) + column;
      snprintf(character_string, sizeof(character_string), "%c", character_number);
      lander_display.drawStr(column * character_width,
                             y_offset + (row * lander_display.getMaxCharHeight()),
                             character_string);
    }
  }
}

void display_test_ascii_2(byte y_offset) {
  drawCenteredString(0, y_offset, "ASCII page 2");
  y_offset += lander_display.getMaxCharHeight();

  // For more characters visible you can uncomment the following line for a smaller font
  // lander_display.setFont(u8g2_font_spleen5x8_mf);
  byte character_width = lander_display.getMaxCharWidth();
  byte column_count = lander_display.getDisplayWidth() / character_width;
  byte row_count = (lander_display.getDisplayHeight() - y_offset) / lander_display.getMaxCharHeight();

  for (byte row = 0; row < row_count; row++) {
    for (byte column = 0; column < column_count; column++) {
      char character_string[2];
      byte character_number = FIRST_PRINTABLE_CHARACTER + 128 + (row * column_count) + column;
      snprintf(character_string, sizeof(character_string), "%c", character_number);
      lander_display.drawStr(column * character_width,
                             y_offset + (row * lander_display.getMaxCharHeight()),
                             character_string);
    }
  }
}

void display_test_extra_page(byte y_offset, byte frame) {
  drawCenteredString(0, y_offset, "Unicode");
  y_offset += lander_display.getMaxCharHeight();
  lander_display.setFont(u8g2_font_unifont_t_symbols);
  lander_display.setFontPosTop();
  lander_display.drawUTF8(0, y_offset, "☀ ☁");
  switch (frame) {
    case 0:
    case 1:
    case 2:
    case 3:
      lander_display.drawUTF8(frame * 3, y_offset + 20, "☂");
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      lander_display.drawUTF8(frame * 3, y_offset + 20, "☔");
      break;
  }
}

const byte CROSS_WIDTH = 24;
const byte CROSS_HEIGHT = 24;
static const unsigned char cross_bits[] U8X8_PROGMEM = {
  0x00, 0x18, 0x00, 0x00, 0x24, 0x00, 0x00, 0x24, 0x00, 0x00, 0x42, 0x00,
  0x00, 0x42, 0x00, 0x00, 0x42, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81, 0x00,
  0xC0, 0x00, 0x03, 0x38, 0x3C, 0x1C, 0x06, 0x42, 0x60, 0x01, 0x42, 0x80,
  0x01, 0x42, 0x80, 0x06, 0x42, 0x60, 0x38, 0x3C, 0x1C, 0xC0, 0x00, 0x03,
  0x00, 0x81, 0x00, 0x00, 0x81, 0x00, 0x00, 0x42, 0x00, 0x00, 0x42, 0x00,
  0x00, 0x42, 0x00, 0x00, 0x24, 0x00, 0x00, 0x24, 0x00, 0x00, 0x18, 0x00, };

const byte CROSS_FILL_WIDTH = 24;
const byte CROSS_FILL_HEIGHT = 24;
static const unsigned char cross_fill_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x64, 0x00, 0x26,
  0x84, 0x00, 0x21, 0x08, 0x81, 0x10, 0x08, 0x42, 0x10, 0x10, 0x3C, 0x08,
  0x20, 0x00, 0x04, 0x40, 0x00, 0x02, 0x80, 0x00, 0x01, 0x80, 0x18, 0x01,
  0x80, 0x18, 0x01, 0x80, 0x00, 0x01, 0x40, 0x00, 0x02, 0x20, 0x00, 0x04,
  0x10, 0x3C, 0x08, 0x08, 0x42, 0x10, 0x08, 0x81, 0x10, 0x84, 0x00, 0x21,
  0x64, 0x00, 0x26, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

const byte CROSS_BLOCK_WIDTH = 14;
const byte CROSS_BLOCK_HEIGHT = 14;
static const unsigned char cross_block_bits[] U8X8_PROGMEM = {
  0xFF, 0x3F, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20,
  0xC1, 0x20, 0xC1, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 0x20,
  0x01, 0x20, 0xFF, 0x3F, };

void display_test_bitmap_modes(byte y_offset, byte frame, bool transparent) {
  if (!transparent) {
    lander_display.setBitmapMode(false);
    drawCenteredString(0, y_offset, "Solid bitmap");
  } else {
    lander_display.setBitmapMode(true);
    drawCenteredString(0, y_offset, "Transparent bitmap");
  }

  y_offset += lander_display.getMaxCharHeight();
  const byte frame_size = CROSS_HEIGHT + 4;
  lander_display.drawBox(0, y_offset,
                         frame_size * 5, frame_size/2);
  lander_display.setDrawColor(0);
  lander_display.drawXBMP(frame_size * 0.5, y_offset + (frame_size/2)-(CROSS_HEIGHT/2),
                          CROSS_WIDTH, CROSS_HEIGHT, cross_bits);
  lander_display.setDrawColor(1);
  lander_display.drawXBMP(frame_size * 2, y_offset + (frame_size/2)-(CROSS_HEIGHT/2),
                          CROSS_WIDTH, CROSS_HEIGHT, cross_bits);
  lander_display.setDrawColor(2);
  lander_display.drawXBMP(frame_size * 3.5, y_offset + (frame_size/2)-(CROSS_HEIGHT/2),
                          CROSS_WIDTH, CROSS_HEIGHT, cross_bits);
  lander_display.setDrawColor(1);

  lander_display.drawStr(frame_size * 0.5, y_offset + frame_size, "Black");
  lander_display.drawStr(frame_size * 2, y_offset + frame_size, "White");
  lander_display.drawStr(frame_size * 3.5, y_offset + frame_size, "XOR");
  if (frame == 7) {
    delay(1000);
  }
}

void display_test_bitmap_overlay(byte y_offset, byte frame) {
  byte frame_size = CROSS_FILL_HEIGHT + 4;
  byte frame_padding = 5;

  drawCenteredString(0, y_offset, "Bitmap overlay");

  y_offset += lander_display.getMaxCharHeight();
  lander_display.setBitmapMode(false);
  lander_display.drawFrame(0, y_offset, frame_size, frame_size);
  lander_display.drawXBMP((frame_size / 2) - (CROSS_FILL_WIDTH / 2),
                          y_offset + (frame_size / 2) - (CROSS_FILL_HEIGHT / 2),
                          CROSS_WIDTH, CROSS_HEIGHT, cross_bits);
  if (frame & 4)
    lander_display.drawXBMP((frame_size / 2) - (CROSS_BLOCK_WIDTH / 2),
                            y_offset + (frame_size / 2) - (CROSS_BLOCK_HEIGHT / 2),
                            CROSS_BLOCK_WIDTH, CROSS_BLOCK_HEIGHT, cross_block_bits);

  lander_display.setBitmapMode(true);
  lander_display.drawFrame(frame_size + frame_padding, y_offset,
                           frame_size, frame_size);
  lander_display.drawXBMP(frame_size + frame_padding + (frame_size / 2) - (CROSS_FILL_WIDTH / 2),
                          y_offset + (frame_size / 2) - (CROSS_FILL_HEIGHT / 2),
                          CROSS_WIDTH, CROSS_HEIGHT, cross_bits);
  if (frame & 4)
    lander_display.drawXBMP(frame_size + frame_padding + (frame_size / 2) - (CROSS_BLOCK_WIDTH / 2),
                            y_offset + (frame_size / 2) - (CROSS_BLOCK_HEIGHT / 2),
                            CROSS_BLOCK_WIDTH, CROSS_BLOCK_HEIGHT, cross_block_bits);

  lander_display.drawStr(0, y_offset + frame_size, "Solid / transparent");
  if (frame == 7) {
    delay(1000);
  }
}
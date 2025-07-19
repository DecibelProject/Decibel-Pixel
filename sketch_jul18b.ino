
// Decibel Pixel Arduino Nano code.

#include <LedControl.h>

// DIN = 11, CLK = 13, CS = 10, 4 
LedControl lc = LedControl(11, 13, 10, 4);

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 8);
    lc.clearDisplay(i);
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "clear all") {
      clearAll();
    } else if (input == "draw all") {
      drawAll();
    } else if (input.startsWith("draw")) {
      processMultiple(input.substring(5), true);
    } else if (input.startsWith("clear")) {
      processMultiple(input.substring(6), false);
    }
  }
}

void clearAll() {
  for (int i = 0; i < 4; i++) {
    lc.clearDisplay(i);
  }
}

void drawAll() {
  for (int i = 0; i < 4; i++) {
    for (int row = 0; row < 8; row++) {
      lc.setRow(i, row, 0xFF);
    }
  }
}

void processMultiple(String data, bool draw) {
  int start = 0;
  while (start < data.length()) {
    int spaceIndex = data.indexOf(' ', start);
    if (spaceIndex == -1) spaceIndex = data.length();

    String coord = data.substring(start, spaceIndex);
    int commaIndex = coord.indexOf(',');
    if (commaIndex != -1) {
      int x = coord.substring(0, commaIndex).toInt() - 1;
      int y = coord.substring(commaIndex + 1).toInt() - 1;

      if (x >= 0 && x < 32 && y >= 0 && y < 8) {
        int mirroredX = 31 - x;

        int device = mirroredX / 8;
        int col = mirroredX % 8;

        lc.setLed(device, y, 7 - col, draw);
      }
    }
    start = spaceIndex + 1;
  }
}
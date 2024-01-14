#include "Line_Detector.h"

#include <pcf8574.h>

#define PCF8574_P0 0
#define PCF8574_P1 1
#define PCF8574_P2 2
#define PCF8574_P3 3
#define PCF8574_P4 4

PCF8574 ex0(0x20);

int LineDetector::begin() {
  pinMode(ex0, PCF8574_P0, INPUT);
  pinMode(ex0, PCF8574_P1, INPUT);
  pinMode(ex0, PCF8574_P2, INPUT);
  pinMode(ex0, PCF8574_P3, INPUT);
  pinMode(ex0, PCF8574_P4, INPUT);
  return 0;
}

uint8_t LineDetector::read() {
  uint8_t flag;
  flag |= digitalRead(ex0, PCF8574_P0);
  flag |= digitalRead(ex0, PCF8574_P1) << 1;
  flag |= digitalRead(ex0, PCF8574_P2) << 2;
  flag |= digitalRead(ex0, PCF8574_P3) << 3;
  flag |= digitalRead(ex0, PCF8574_P4) << 4;
  return flag;
}

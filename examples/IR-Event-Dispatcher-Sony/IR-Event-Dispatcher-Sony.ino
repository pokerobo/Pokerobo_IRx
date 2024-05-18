#include "Pokerobo_IRx.h"

IRDebugLogger irDebugLogger;
IRSerialPrintDispatcher eventDispatcher;

IRCodeMapping mappingSony(23u, (uint16_t[]) { // SONY (23)
    0x74,  // BIT_UP_BUTTON
    0x33,  // BIT_RIGHT_BUTTON
    0x75,  // BIT_DOWN_BUTTON
    0x34,  // BIT_LEFT_BUTTON
    0x65,  // BIT_OK_BUTTON
    0x3A,  // BIT_ASTERISK_BUTTON
    0x3F,  // BIT_SHARP_BUTTON
    0x09,  // BIT_DIGIT0_BUTTON
    0x00,  // BIT_DIGIT1_BUTTON
    0x01,  // BIT_DIGIT2_BUTTON
    0x02,  // BIT_DIGIT3_BUTTON
    0x03,  // BIT_DIGIT4_BUTTON
    0x04,  // BIT_DIGIT5_BUTTON
    0x05,  // BIT_DIGIT6_BUTTON
    0x06,  // BIT_DIGIT7_BUTTON
    0x07,  // BIT_DIGIT8_BUTTON
    0x08,  // BIT_DIGIT9_BUTTON
  }, 17u);
IRCodeMapper irCodeMapper((IRCodeMapping* []) { &mappingSony }, 1);
IRListener irListener(&irCodeMapper, &eventDispatcher);

void setup() {
  Serial.begin(57600);
  eventDispatcher.set(&irDebugLogger);
  irListener.begin(A2);
}

void loop() {
  uint32_t startTime = millis();
  irListener.loop();
  delay(max(50 + startTime - millis(), 0));
}

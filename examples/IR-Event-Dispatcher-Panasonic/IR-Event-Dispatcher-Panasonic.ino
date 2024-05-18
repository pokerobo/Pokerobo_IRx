#include "Pokerobo_IRx.h"

IRDebugLogger irDebugLogger;
IRSerialPrintDispatcher eventDispatcher;

IRCodeMapper irCodeMapper;
IRListener irListener(&irCodeMapper, &eventDispatcher);

void setup() {
  Serial.begin(57600);

  uint16_t mappingPanasonic[] = { // PANASONIC - 11
    0x34,  // BIT_UP_BUTTON
    0x20,  // BIT_RIGHT_BUTTON
    0x35,  // BIT_DOWN_BUTTON
    0x21,  // BIT_LEFT_BUTTON
    0x49,  // BIT_OK_BUTTON
    0x39,  // BIT_ASTERISK_BUTTON
    0x3B,  // BIT_SHARP_BUTTON
    0x19,  // BIT_DIGIT0_BUTTON
    0x10,  // BIT_DIGIT1_BUTTON
    0x11,  // BIT_DIGIT2_BUTTON
    0x12,  // BIT_DIGIT3_BUTTON
    0x13,  // BIT_DIGIT4_BUTTON
    0x14,  // BIT_DIGIT5_BUTTON
    0x15,  // BIT_DIGIT6_BUTTON
    0x16,  // BIT_DIGIT7_BUTTON
    0x17,  // BIT_DIGIT8_BUTTON
    0x18,  // BIT_DIGIT9_BUTTON
  };
  irCodeMapper.addMapping(11u, mappingPanasonic, 17u);

  eventDispatcher.set(&irDebugLogger);
  irListener.begin(A2);
}

void loop() {
  uint32_t startTime = millis();
  irListener.loop();
  delay(max(50 + startTime - millis(), 0));
}

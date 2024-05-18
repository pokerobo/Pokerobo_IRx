#include "Pokerobo_IRx.h"

IRDebugLogger irDebugLogger;
IRSerialPrintDispatcher eventDispatcher;

IRCodeMapper irCodeMapper;
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

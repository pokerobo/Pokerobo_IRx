#include "Pokerobo_Car.h"

const uint64_t address = 0x18580901LL;
RF24Controller rf24Controller(address);
HangingDetector hangingDetector;

RoboCarHandler roboCarHandler;
MovingResolver movingResolver;

EventListener eventListener;
EventTrigger eventTrigger;

void setup() {
  while (!Serial) delay(100); // Wait for the serial connection to be establised.
  Serial.begin(57600);

  #if __LOADING_LOG_ENABLED__
  debugLog("main", "()", " - ", "Starting");
  #endif

  roboCarHandler.set(&movingResolver);
  roboCarHandler.begin();

  eventTrigger.set(&roboCarHandler);
  eventTrigger.begin();

  hangingDetector.begin([] (void ()) {
    roboCarHandler.stop();
  }, 100);

  rf24Controller.set(&hangingDetector);
  rf24Controller.begin();

  eventListener.set(&eventTrigger);
  eventListener.set(&rf24Controller);
  eventListener.begin();

  #if __LOADING_LOG_ENABLED__
  debugLog("main", "()", " - ", "Done!");
  #endif
}

void loop() {
  uint32_t begin = millis();

  eventListener.check();

  uint32_t exectime = millis() - begin;
  // Serial.print("EXECTIME"), Serial.print(": "), Serial.print(exectime), Serial.println();

  delay(max(100 - exectime, 0));
}

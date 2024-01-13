#include "Pokerobo_Car.h"

const uint64_t address = 0x18580901LL;
RF24Listener rf24Listener(address);
HangingDetector hangingDetector;

RoboCarHandler roboCarHandler;
MovingResolver movingResolver;

ProgramManager programManager;
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

  rf24Listener.set(&hangingDetector);
  rf24Listener.begin();

  programManager.set(&eventTrigger);
  programManager.set(&rf24Listener);
  programManager.begin();

  #if __LOADING_LOG_ENABLED__
  debugLog("main", "()", " - ", "Done!");
  #endif
}

void loop() {
  uint32_t begin = millis();

  programManager.check();

  uint32_t exectime = millis() - begin;
  // Serial.print("EXECTIME"), Serial.print(": "), Serial.print(exectime), Serial.println();

  delay(max(100 - exectime, 0));
}

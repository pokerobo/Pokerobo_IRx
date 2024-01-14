#include "Pokerobo_Car.h"

DisplayAdapter displayAdapter;

RF24Listener rf24Listener(0x18580901LL);
HangingDetector hangingDetector;

RoboCarHandler roboCarHandler;
MovingResolver movingResolver;

LineFollowingCar lineFollowingCar(" Line Following Car");

ProgramManager programManager;

void setup() {
  while (!Serial) delay(100); // Wait for the serial connection to be establised.
  Serial.begin(57600);

  displayAdapter.begin();

  roboCarHandler.set(&movingResolver);
  roboCarHandler.begin();

  lineFollowingCar.set(&displayAdapter);
  lineFollowingCar.set(&roboCarHandler);
  lineFollowingCar.begin();

  hangingDetector.begin([] (void ()) {
    displayAdapter.clear();
    displayAdapter.render(0, 0, "Suspending...");
    roboCarHandler.stop();
  }, 10);

  rf24Listener.begin();

  programManager.set(&rf24Listener);
  programManager.set(&displayAdapter);
  programManager.set(&hangingDetector);
  programManager.add(&lineFollowingCar);
  programManager.begin();
}

void loop() {
  uint32_t begin = millis();
  programManager.check();
  delay(max(100 - (millis() - begin), 0));
}

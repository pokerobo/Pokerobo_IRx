#include "Pokerobo_Car.h"

#if (CONTROLLER_IR)
#if (CONTROLLER_IR_DEVICE_SONY)
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
#else
IRCodeMapper irCodeMapper;
#endif

IRController irController(&irCodeMapper);
#endif

#if (CONTROLLER == CONTROLLER_PS2)
PS2Controller ps2Controller;
#endif

#if (CONTROLLER == CONTROLLER_RF24)
const uint64_t address = 0x18580901LL;
RF24Controller rf24Controller(address);
#endif

#if (CONTROLLER_ROBOCAR)
RoboCarHandler roboCarHandler;
MovingResolver movingResolver;
#endif

PedestalHandler pedestalHandler1(4, 5);
PedestalHandler pedestalHandler3(8, 9);

PedestalHandler* pedestalHandlers[PEDESTALS_MAX] = {
  &pedestalHandler1,
  &pedestalHandler3,
};

PedestalGroup pedestalGroup(pedestalHandlers);

EventTrigger eventTrigger;

#if (CONTROLLER == CONTROLLER_RF24)
HangingDetector hangingDetector;
#endif

void setup() {
  while (!Serial) delay(100); // Wait for the serial connection to be establised.
  Serial.begin(57600);

#if __LOADING_LOG_ENABLED__
  debugLog("main", "()", " - ", "Starting");
#endif

#if (CONTROLLER_ROBOCAR)
  roboCarHandler.begin();
  roboCarHandler.set(&movingResolver);
  eventTrigger.set(&roboCarHandler);
#endif

  pedestalGroup.begin();
  eventTrigger.set(&pedestalGroup);

#if (CONTROLLER == CONTROLLER_PS2)
  ps2Controller.begin();
  ps2Controller.set(&eventTrigger);
#endif

#if (CONTROLLER == CONTROLLER_RF24)
  hangingDetector.begin([] (void ()) {
#if (CONTROLLER_ROBOCAR)
    roboCarHandler.stop();
#endif
  }, 100);
#endif

#if (CONTROLLER == CONTROLLER_RF24)
  rf24Controller.begin();
  rf24Controller.set(&hangingDetector);
  rf24Controller.set(&eventTrigger);
#endif

#if (CONTROLLER_IR)
#if (CONTROLLER_IR_DEVICE_PANASONIC)
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
#endif // CONTROLLER_IR_DEVICE_PANASONIC
#endif // CONTROLLER_IR

#if (CONTROLLER_IR)
  irController.begin();
  irController.set(&eventTrigger);
#endif

#if __LOADING_LOG_ENABLED__
  debugLog("main", "()", " - ", "Done!");
#endif
}

void loop() {
  uint32_t begin = millis();

  eventTrigger.check();

#if (CONTROLLER == CONTROLLER_PS2)
  ps2Controller.loop();
#endif
#if (CONTROLLER == CONTROLLER_RF24)
  rf24Controller.loop();
#endif
#if (CONTROLLER_IR)
  irController.loop();
#endif
  uint32_t exectime = millis() - begin;
  // Serial.print("EXECTIME"), Serial.print(": "), Serial.print(exectime), Serial.println();
  delay(max(100 - exectime, 0));
}

#include "Commons.h"

#if (CONTROLLER_IR)
#include "IR_Controller.h"
#endif

#if (CONTROLLER == CONTROLLER_PS2)
#include "PS2_Controller.h"
#endif

#if (CONTROLLER == CONTROLLER_RF24)
#include "RF24_Controller.h"
#endif

#if (CONTROLLER_CARBOT)
#include "Carbot_Handler.h"
#endif

#include "Pedestal_Group.h"

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
IRCodeMapper irCodeMapperInstance((IRCodeMapping* []) { &mappingSony }, 1);
#else
IRCodeMapper irCodeMapperInstance;
#endif

IRCodeMapper* irCodeMapper = &irCodeMapperInstance;

IRController irControllerInstance(irCodeMapper);
IRController* irController = &irControllerInstance;
#endif

#if (CONTROLLER == CONTROLLER_PS2)
PS2Controller ps2ControllerInstance;
PS2Controller* ps2Controller = &ps2ControllerInstance;
#endif

#if (CONTROLLER == CONTROLLER_RF24)
RF24Controller rf24ControllerInstance;
RF24Controller* rf24Controller = &rf24ControllerInstance;
#endif

#if (CONTROLLER_CARBOT)
CarbotHandler carbotHandler;
#endif

PedestalHandler pedestalHandler1(4, 5);
PedestalHandler pedestalHandler3(8, 9);

PedestalHandler* pedestalHandlers[PEDESTALS_MAX] = {
  &pedestalHandler1,
  &pedestalHandler3,
};

PedestalGroup pedestalGroupInstance(pedestalHandlers);
PedestalGroup* pedestalGroup = &pedestalGroupInstance;

int program = 0;

void setup() {
  while (!Serial) {
    delay(100); // Wait for the serial connection to be establised.
  }
  Serial.begin(57600);

#if __LOADING_LOG_ENABLED__
  debugLog("main", "()", " - ", "Starting");
#endif

#if (CONTROLLER_CARBOT)
  carbotHandler.begin();
#endif

  pedestalGroup->begin();

#if (CONTROLLER == CONTROLLER_PS2)
  ps2Controller->begin();

  ps2Controller->setOnStartButtonPressed(processStartButtonPressedEvent);

  ps2Controller->setOnDPadUpButtonPressed(processDPadUpButtonPressedEvent);
  ps2Controller->setOnDPadRightButtonPressed(processDPadRightButtonPressedEvent);
  ps2Controller->setOnDPadDownButtonPressed(processDPadDownButtonPressedEvent);
  ps2Controller->setOnDPadLeftButtonPressed(processDPadLeftButtonPressedEvent);

  ps2Controller->setOnnLeftJoystickChanged(processLeftJoystickChangeEvent);
  ps2Controller->setOnRightJoystickChanged(processRightJoystickChangeEvent);
#endif

#if (CONTROLLER == CONTROLLER_RF24)
  rf24Controller->begin();

  rf24Controller->setOnStartButtonPressed(processStartButtonPressedEvent);
  rf24Controller->setOnAnalogButtonPressed(processAnalogButtonPressedEvent);

  rf24Controller->setOnDPadUpButtonPressed(processDPadUpButtonPressedEvent);
  rf24Controller->setOnDPadRightButtonPressed(processDPadRightButtonPressedEvent);
  rf24Controller->setOnDPadDownButtonPressed(processDPadDownButtonPressedEvent);
  rf24Controller->setOnDPadLeftButtonPressed(processDPadLeftButtonPressedEvent);

  rf24Controller->setOnnLeftJoystickChanged(processLeftJoystickChangeEvent);
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
  irCodeMapper->addMapping(11u, mappingPanasonic, 17u);
#endif // CONTROLLER_IR_DEVICE_PANASONIC
#endif // CONTROLLER_IR

#if (CONTROLLER_IR)
  irController->begin();

  irController->setOnDPadUpButtonPressed(processDPadUpButtonPressedEvent);
  irController->setOnDPadRightButtonPressed(processDPadRightButtonPressedEvent);
  irController->setOnDPadDownButtonPressed(processDPadDownButtonPressedEvent);
  irController->setOnDPadLeftButtonPressed(processDPadLeftButtonPressedEvent);
#endif

#if __LOADING_LOG_ENABLED__
  debugLog("main", "()", " - ", "Done!");
#endif
}

void loop() {
  uint32_t begin = millis();
  if (program == 1) {
    pedestalGroup->autoDance();
  }
#if (CONTROLLER == CONTROLLER_PS2)
  getDelayAmount(ps2Controller->loop());
#endif
#if (CONTROLLER == CONTROLLER_RF24)
  getDelayAmount(rf24Controller->loop());
#endif
#if (CONTROLLER_IR)
  getDelayAmount(irController->loop());
#endif
  uint32_t exectime = millis() - begin;
  // Serial.print("EXECTIME"), Serial.print(": "), Serial.print(exectime), Serial.println();
  delay(max(100 - exectime, 0));
}

uint32_t getDelayAmount(int status) {
  if (status >= 1) {
    return 5;
  } else {
    return 100;
  }
}

int switchProgram() {
  program = 1 - program;
  if (program == 0) {
    pedestalGroup->reset();
#if (CONTROLLER_CARBOT)
    carbotHandler.turnOff();
#endif
  }
  if (program == 1) {
#if (CONTROLLER_CARBOT)
    carbotHandler.turnOn();
#endif
  }
}

void processStartButtonPressedEvent() {
  pedestalGroup->processStartButtonPressedEvent();
}

void processAnalogButtonPressedEvent() {
  switchProgram();
  pedestalGroup->processAnalogButtonPressedEvent();
}

void processDPadUpButtonPressedEvent() {
  pedestalGroup->processDPadUpButtonPressedEvent();
}

void processDPadRightButtonPressedEvent() {
  pedestalGroup->processDPadRightButtonPressedEvent();
}

void processDPadDownButtonPressedEvent() {
  pedestalGroup->processDPadDownButtonPressedEvent();
}

void processDPadLeftButtonPressedEvent() {
  pedestalGroup->processDPadLeftButtonPressedEvent();
}

void processLeftJoystickChangeEvent(int nJoyX, int nJoyY) {
  if (program == 1) {
#if (CONTROLLER_CARBOT)
  carbotHandler.move(nJoyX, nJoyY);
#endif
  } else {
    pedestalGroup->processLeftJoystickChangeEvent(nJoyX, nJoyY);
  }
}

void processRightJoystickChangeEvent(int nJoyX, int nJoyY) {
  pedestalGroup->processRightJoystickChangeEvent(nJoyX, nJoyY);
}

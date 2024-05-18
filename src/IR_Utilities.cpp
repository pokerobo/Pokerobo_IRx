#include "IR_Utilities.h"

void IRSerialPrintDispatcher::processOkButtonPressedEvent() {
  Serial.println("Button: Ok");
}

void IRSerialPrintDispatcher::processDPadUpButtonPressedEvent() {
  Serial.println("Button: Up");
}

void IRSerialPrintDispatcher::processDPadRightButtonPressedEvent() {
  Serial.println("Button: Right");
}

void IRSerialPrintDispatcher::processDPadDownButtonPressedEvent() {
  Serial.println("Button: Down");
}

void IRSerialPrintDispatcher::processDPadLeftButtonPressedEvent() {
  Serial.println("Button: Left");
}

void IRSerialPrintDispatcher::processAsteriskButtonPressedEvent() {
  Serial.println("Button: Asterisk");
}

void IRSerialPrintDispatcher::processSharpButtonPressedEvent() {
  Serial.println("Button: Sharp");
}

void IRSerialPrintDispatcher::processDigitButtonsPressedEvent(uint32_t buttons) {
  Serial.print("DigitButtons: "), Serial.println(buttons, BIN);
}

void IRSerialPrintDispatcher::processOtherButtonsPressedEvent(uint16_t buttons) {
  Serial.println("OtherButtons: "), Serial.println(buttons, BIN);
}

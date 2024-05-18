#include "IR_Listener.h"
#include <IRremote.hpp>

uint32_t detectButtonPress(IRData, IRCodeMapper*);

IRListener::IRListener(IRCodeMapper* irCodeMapper, IRProcessor* irProcessor) {
  _debugEnabled = true;
  _irCodeMapper = irCodeMapper;
  _irProcessor = irProcessor;
}

void IRListener::set(IRCodeMapper* irCodeMapper) {
  _irCodeMapper = irCodeMapper;
}

void IRListener::set(IRProcessor* irProcessor) {
  _irProcessor = irProcessor;
};

void IRListener::begin(uint8_t aReceivePin) {
  IrReceiver.begin(aReceivePin, ENABLE_LED_FEEDBACK);
  //
  #if __LOADING_LOG_ENABLED__
  if (isDebugEnabled()) {
    char buf[7];
    _logger->debug("IR is now running on Pin", " ", itoa(RECV_PIN, buf, 10));
  }
  #endif
}

int IRListener::loop() {
  if (!IrReceiver.decode()) {
    return 0;
  }

  uint32_t buttons = detectButtonPress(IrReceiver.decodedIRData, _irCodeMapper);
  uint32_t checked = processButtonPress(buttons);

  IrReceiver.resume();

  return checked ? 1 : -1;
}

uint32_t IRListener::processButtonPress(uint32_t buttons) {
  if (_irProcessor == NULL) {
    return 0;
  }
  return _irProcessor->processButtonPress(buttons);
}

//-------------------------------------------------------------------------------------------------

uint32_t detectButtonPress(IRData decodedIRData, IRCodeMapper* _irCodeMapper) {
  uint32_t buttons = 0;

  if (_irCodeMapper != NULL) {
    uint8_t pos = _irCodeMapper->getPosition((uint8_t)decodedIRData.protocol, decodedIRData.command);
    if (_irCodeMapper->isValid(pos)) {
      buttons |= (1UL << pos);
      #if __RUNNING_LOG_ENABLED__
      char pos_[7];
      IRDebugLogger::debug("Position", ": ", itoa(pos, pos_, 10), " is pushed");
      Serial.print("Flags: "), Serial.println(buttons, BIN);
      #endif
      return buttons;
    } else {
      #if __RUNNING_LOG_ENABLED__
      IrReceiver.printIRResultShort(&Serial);
      #endif
    }
  }

  return buttons;
}

void show(IRData decodedIRData, HardwareSerial *serial) {
  switch (decodedIRData.protocol) {
    case NEC: Serial.println("NEC"); break;
    case SONY: Serial.println("SONY"); break;
    case RC5: Serial.print("RC"), Serial.println("5"); break;
    case RC6: Serial.print("RC"), Serial.println("6"); break;
    case SHARP: Serial.println("SHARP"); break;
    case JVC: Serial.println("JVC"); break;
    case SAMSUNG: Serial.println("SAMSUNG"); break;
    case LG: Serial.println("LG"); break;
    case WHYNTER: Serial.println("WHYNTER"); break;
    case PANASONIC: Serial.println("PANASONIC"); break;
    case DENON: Serial.println("DENON"); break;
    case UNKNOWN: Serial.println("UNKNOWN"); break;
    default:
      Serial.print("Unknown [");
      Serial.print(decodedIRData.protocol);
      Serial.println("]");
  }
}

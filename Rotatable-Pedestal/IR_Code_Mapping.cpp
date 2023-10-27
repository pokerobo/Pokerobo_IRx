#include "IR_Code_Mapping.h"

IRCodeMapping::IRCodeMapping() {}

IRCodeMapping::IRCodeMapping(uint8_t type, uint16_t map[], uint8_t len) {
  _protocol = type;
  for (uint8_t i=0; i<len; i++) {
    _codes[i] = map[i];
  }
  _length = len;
}

uint8_t IRCodeMapping::getType() {
  return _protocol;
}

uint8_t IRCodeMapping::getLength() {
  return _length;
}

bool IRCodeMapping::hasCode(uint16_t code) {
  return getPosition(code) < CODE_NUMBER;
}

uint8_t IRCodeMapping::getPosition(uint16_t code) {
  for (uint8_t i=0; i<CODE_NUMBER; i++) {
    if (_codes[i] == code) {
      return i;
    }
  }
  return CODE_NUMBER;
}

IRCodeMapper::IRCodeMapper() {
  initialize();
}

IRCodeMapper::IRCodeMapper(IRCodeMapping* mappings[], uint8_t amount) {
  initialize();
  for (uint8_t i=0; i<amount; i++) {
    if (mappings[i] == NULL) {
      continue;
    }
    addMapping(mappings[i]);
  }
}

void IRCodeMapper::initialize() {
  uint16_t mapping[] = { // NEC (8)
    0x18,  // BIT_UP_BUTTON
    0x5A,  // BIT_RIGHT_BUTTON
    0x52,  // BIT_DOWN_BUTTON
    0x08,  // BIT_LEFT_BUTTON
    0x1C,  // BIT_OK_BUTTON
    0x16,  // BIT_ASTERISK_BUTTON
    0x0D,  // BIT_SHARP_BUTTON
    0x19,  // BIT_DIGIT0_BUTTON
    0x45,  // BIT_DIGIT1_BUTTON
    0x46,  // BIT_DIGIT2_BUTTON
    0x47,  // BIT_DIGIT3_BUTTON
    0x44,  // BIT_DIGIT4_BUTTON
    0x40,  // BIT_DIGIT5_BUTTON
    0x43,  // BIT_DIGIT6_BUTTON
    0x07,  // BIT_DIGIT7_BUTTON
    0x15,  // BIT_DIGIT8_BUTTON
    0x09,  // BIT_DIGIT9_BUTTON
  };
  addMapping(8u, mapping, 17u);
}

bool IRCodeMapper::addMapping(uint8_t type, uint16_t map[], uint8_t len) {
  return addMapping(new IRCodeMapping(type, map, len));
}

bool IRCodeMapper::addMapping(IRCodeMapping* mapping) {
  if (mapping == NULL) {
    return false;
  }
  if (_codeMappingsTotal >= CODE_MAPPINGS_MAX) {
    return false;
  }
  _codeMappings[_codeMappingsTotal] = mapping;
  _codeMappingsTotal++;
  return true;
}

uint8_t IRCodeMapper::getPosition(uint8_t protocol, uint16_t command) {
  IRCodeMapping* codeMapping = NULL;

  for (uint8_t i=0; i<_codeMappingsTotal; i++) {
    IRCodeMapping* cm = _codeMappings[i];
    if (cm->getType() == protocol) {
      codeMapping = cm;
      break;
    }
  }

  if (codeMapping == NULL) {
    return CODE_NUMBER;
  }

  return codeMapping->getPosition(command);
}

bool IRCodeMapper::isValid(uint8_t position) {
  return position < CODE_NUMBER;
}

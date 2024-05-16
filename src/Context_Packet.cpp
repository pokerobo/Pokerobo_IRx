#include "Context_Packet.h"

#ifndef __POKEROBO_RC_CLIENT_H__
MasterContext::MasterContext() {}

MasterContext::MasterContext(uint8_t applicationId, bool programCheckBit) {
  _applicationId = applicationId;
  _programCheckBit = programCheckBit;

  uint8_t checkBit = 0;
  for(uint8_t i=0; i<6; i++) {
    checkBit ^= (_applicationId >> i) & 0b1;
  }
  checkBit ^= _programCheckBit & 0b1;

  _contextCheckBit = checkBit;
}

MasterContext::MasterContext(uint8_t* buf) {
  deserialize(buf);
}

uint8_t MasterContext::getApplicationId() {
  return _applicationId;
}

bool MasterContext::getContextCheckBit() {
  return _contextCheckBit;
}

bool MasterContext::getProgramCheckBit() {
  return _programCheckBit;
}

const uint8_t MasterContext::messageSize = sizeof(uint16_t);

uint8_t MasterContext::length() {
  return messageSize;
}

uint8_t* MasterContext::serialize(uint8_t* buf, uint8_t len) {
  if (len < messageSize) {
    return NULL;
  }

  uint8_t data = 0;
  data |= (_applicationId & ((1U << 6) - 1));
  data |= _contextCheckBit << 7;
  data |= _programCheckBit << 6;
  buf[0] = data;

  buf[1] = 0; // reserved

  return buf;
}

MasterContext* MasterContext::deserialize(uint8_t* buf) {
  if (buf == NULL) {
    return NULL;
  }

  _contextCheckBit = (buf[0] >> 7) & 0b1;
  _programCheckBit = (buf[0] >> 6) & 0b1;
  _applicationId = buf[0] & ((1U << 6) - 1);
}
#endif

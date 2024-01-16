#ifndef __MESSAGE_EXCHANGE_H__
#define __MESSAGE_EXCHANGE_H__

#include "Commons.h"

class MessageInterface {
  public:
    uint8_t getVariation();
    void setVariation(uint8_t variation);
    virtual uint8_t length();
    virtual void* deserialize(uint8_t* buf);
  private:
    uint8_t _variation = 0;
};

uint8_t* encodeInteger(uint8_t* store, uint16_t value);
uint8_t* encodeInteger(uint8_t* store, uint32_t value);
uint32_t decodeInteger(uint8_t* arr, int length);

#endif

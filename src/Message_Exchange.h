#ifndef __POKEROBO_CAR_MESSAGE_EXCHANGE_H__
#define __POKEROBO_CAR_MESSAGE_EXCHANGE_H__

#include "Commons.h"

#if __POKEROBO_RCB_CLIENT_ENABLED__
#include "Pokerobo_RCB_client.h"
#endif//__POKEROBO_RCB_CLIENT_ENABLED__

#ifndef __POKEROBO_RC_CLIENT_H__
class MessageInterface {
  public:
    uint8_t getVariation();
    void setVariation(uint8_t variation);
    virtual uint8_t length();
    virtual uint8_t* serialize(uint8_t* buf, uint8_t len);
    virtual void* deserialize(uint8_t* buf);
  private:
    uint8_t _variation = 0;
};

uint8_t* encodeInteger(uint8_t* store, uint16_t value);
uint8_t* encodeInteger(uint8_t* store, uint32_t value);
uint32_t decodeInteger(uint8_t* arr, int length);
#endif//__POKEROBO_RC_CLIENT_H__

#endif

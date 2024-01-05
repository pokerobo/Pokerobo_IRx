#ifndef __MESSAGE_EXCHANGE_H__
#define __MESSAGE_EXCHANGE_H__

#include "Commons.h"

uint8_t* encodeInteger(uint8_t* store, uint16_t value);
uint8_t* encodeInteger(uint8_t* store, uint32_t value);
uint32_t decodeInteger(uint8_t* arr, int length);

#endif

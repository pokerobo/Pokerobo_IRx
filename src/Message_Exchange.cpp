#include "Message_Exchange.h"

uint8_t* encodeInteger(uint8_t* store, uint16_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  return store;
}

uint8_t* encodeInteger(uint8_t* store, uint32_t value) {
  store[0] = value & 0xff;
  store[1] = (value >> 8) & 0xff;
  store[2] = (value >> 16) & 0xff;
  store[3] = (value >> 24) & 0xff;
  return store;
}

uint32_t decodeInteger(uint8_t* arr, int length) {
  if (length == 2) {
    uint16_t a1 = arr[1];
    return (a1 << 8) | arr[0];
  }
  if (length == 4) {
    uint16_t a1 = arr[1];
    uint32_t a2 = arr[2];
    uint32_t a3 = arr[3];
    return (a3 << 24) | (a2 << 16) | (a1 << 8) | arr[0];
  }
  return 0;
}
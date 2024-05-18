#ifndef __POKEROBO_IRX_IR_CODE_MAPPING_H__
#define __POKEROBO_IRX_IR_CODE_MAPPING_H__

#include <Arduino.h>

#define CODE_NUMBER 32
#define CODE_MAPPINGS_MAX 10

class IRCodeMapping {
  public:
    IRCodeMapping(uint8_t type, uint16_t map[], uint8_t len);
    uint8_t getType();
    uint8_t getLength();
    uint8_t getPosition(uint16_t code);
    bool hasCode(uint16_t code);
  private:
    IRCodeMapping();
    uint8_t _protocol;
    uint16_t _codes[CODE_NUMBER] = {};
    uint8_t _length = 0;
};

class IRCodeMapper {
  public:
    IRCodeMapper();
    IRCodeMapper(IRCodeMapping* mappings[], uint8_t amount);
    bool addMapping(uint8_t type, uint16_t map[], uint8_t len);
    bool addMapping(IRCodeMapping* mapping);
    uint8_t getPosition(uint8_t protocol, uint16_t command);
    bool isValid(uint8_t position);
  private:
    void initialize();
    uint8_t _codeMappingsTotal = 0;
    IRCodeMapping* _codeMappings[CODE_MAPPINGS_MAX] = {};
};

#endif

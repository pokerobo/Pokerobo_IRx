#ifndef __CONTEXT_PACKET_H__
#define __CONTEXT_PACKET_H__

#include "Commons.h"
#include "Message_Exchange.h"

#ifndef __POKEROBO_RC_CLIENT_H__
class MasterContext: public MessageInterface {
  public:
    MasterContext();
    MasterContext(uint8_t applicationId, bool programCheckBit);
    MasterContext(uint8_t* buf);
    uint8_t getApplicationId();
    bool getContextCheckBit();
    bool getProgramCheckBit();

    static const uint8_t messageSize;
    uint8_t length();
    uint8_t* serialize(uint8_t* buf, uint8_t len);
    MasterContext* deserialize(uint8_t* buf);
  private:
    int _errorCode = 0;
    bool _debugEnabled = true;
    uint8_t _applicationId = 0;
    bool _contextCheckBit = 0;
    bool _programCheckBit = 0;
};
#endif

#endif

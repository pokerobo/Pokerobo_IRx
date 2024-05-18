#ifndef __POKEROBO_IRX_IR_LISTENER_H__
#define __POKEROBO_IRX_IR_LISTENER_H__

#include "IR_Commons.h"
#include "IR_Mapping_Code.h"
#include "IR_Processor.h"

#ifndef POKEROBO_IRX_RECV_PIN
#define POKEROBO_IRX_RECV_PIN                 2
#endif

class IRListener: public IRProcessor {
  public:
    IRListener(IRCodeMapper* irCodeMapper=NULL, IRProcessor* irProcessor=NULL);
    void set(IRCodeMapper* irCodeMapper);
    void set(IRProcessor* irProcessor);
    void begin(uint8_t aReceivePin=POKEROBO_IRX_RECV_PIN);
    int loop();
    uint32_t processButtonPress(uint32_t buttons);
  private:
    IRCodeMapper* _irCodeMapper = NULL;
    IRProcessor* _irProcessor = NULL;
};

#endif

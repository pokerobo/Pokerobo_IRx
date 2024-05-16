#ifndef __POKEROBO_CAR_PROGRAM_CAPSULE_H__
#define __POKEROBO_CAR_PROGRAM_CAPSULE_H__

#include "Commons.h"
#include "Pokerobo_RCB_client.h"

#ifndef __POKEROBO_RC_CLIENT_H__
class ProgramCapsule {
  public:
    virtual char* getTitle();
    virtual int begin();
    virtual int check(void* action=NULL, void* command=NULL);
    virtual int close();
};

#endif//#ifndef __POKEROBO_RC_CLIENT_H__
#endif

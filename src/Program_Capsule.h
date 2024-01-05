#ifndef __PROGRAM_CAPSULE_H__
#define __PROGRAM_CAPSULE_H__

#include "Commons.h"

class ProgramCapsule {
  public:
    virtual char* getTitle();
    virtual int begin();
    virtual int check(void* action, void* command);
    virtual int close();
};

#endif

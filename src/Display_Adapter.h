#ifndef __DISPLAY_ADAPTER_H__
#define __DISPLAY_ADAPTER_H__

#include "Program_Collection.h"

class DisplayAdapter {
  public:
    DisplayAdapter();
    void begin();
    void render(ProgramCollection* programCollection);
    void clear();
  protected:
    void* _lcdRef = NULL;
};

#endif

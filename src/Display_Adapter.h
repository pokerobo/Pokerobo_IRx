#ifndef __DISPLAY_ADAPTER_H__
#define __DISPLAY_ADAPTER_H__

#include "Program_Collection.h"

class DisplayAdapter {
  public:
    DisplayAdapter();
    void begin();
    void render(ProgramCollection* programCollection);
    void render(int16_t col, int16_t row, char* text);
    void clear();
  protected:
    void* _lcdRef = NULL;
};

#endif

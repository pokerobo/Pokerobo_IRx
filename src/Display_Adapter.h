#ifndef __DISPLAY_ADAPTER_H__
#define __DISPLAY_ADAPTER_H__

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

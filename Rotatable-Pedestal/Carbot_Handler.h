#ifndef __CARBOT_2WHEELS_H__
#define __CARBOT_2WHEELS_H__

#ifndef __CARBOT_LOADING_LOG__
#define __CARBOT_LOADING_LOG__  __LOADING_LOG_ENABLED__
#endif//__CARBOT_LOADING_LOG__

#ifndef __CARBOT_RUNNING_LOG__
#define __CARBOT_RUNNING_LOG__  __RUNNING_LOG_ENABLED__
#endif//__CARBOT_RUNNING_LOG__

#define DEFAULT_SPEED 128
#define DEFAULT_COEFF 5

#include "Commons.h"

class CarbotHandler {
  public:
    int begin();
    bool isActive();
    void turnOn();
    void turnOff();
    void flip();
    void stop();
    void move(int x, int y, int coeff=1, bool rotatable=false);
    void moveForward(int speed=DEFAULT_SPEED);
    void turnLeft(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED);
    void turnRight(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED);
    void moveBack(int speed=DEFAULT_SPEED);
    void backLeft(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED);
    void backRight(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED);
    void rotateLeft(int speed=DEFAULT_SPEED);
    void rotateRight(int speed=DEFAULT_SPEED);
  private:
    bool _active = false;
};

#endif

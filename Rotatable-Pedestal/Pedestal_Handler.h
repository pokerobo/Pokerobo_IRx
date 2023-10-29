#ifndef __PEDESTAL_HANDLER_H__
#define __PEDESTAL_HANDLER_H__

#include "Commons.h"

#ifndef __HCPCA9685_ENABLED__
#define __HCPCA9685_ENABLED__   1
#endif

#if !__HCPCA9685_ENABLED__
#include <Servo.h>
#else
#include "HCPCA9685.h"
#endif

#ifndef HORIZONTAL_SERVO_PIN
#define HORIZONTAL_SERVO_PIN  4
#endif

#ifndef VERTICAL_SERVO_PIN
#define VERTICAL_SERVO_PIN    5
#endif

#ifndef HORIZONTAL_MIN_ANGLE
#define HORIZONTAL_MIN_ANGLE  30
#endif

#ifndef HORIZONTAL_MAX_ANGLE
#define HORIZONTAL_MAX_ANGLE  120
#endif

#ifndef VERTICAL_MIN_ANGLE
#define VERTICAL_MIN_ANGLE    30
#endif

#ifndef VERTICAL_MAX_ANGLE
#define VERTICAL_MAX_ANGLE    90
#endif

#ifndef MOVING_AMOUNT
#define MOVING_AMOUNT         5
#endif

class PedestalHandler {
  public:
    static void init();
    static bool initialized;

    PedestalHandler(byte hPin=HORIZONTAL_SERVO_PIN, byte vPin=VERTICAL_SERVO_PIN, bool debugEnabled=true);
    void begin(int hMinAngle=0, int hMaxAngle=180, int vMinAngle=0, int vMaxAngle=180);
    void reset();
    void test();

    bool verticalServoUp();
    bool verticalServoDown();
    bool horizontalServoLeft();
    bool horizontalServoRight();  

    int changeHorizontalServo(int hDelta);
    int changeVerticalServo(int vDelta);
    bool change(int hDelta, int vDelta);
    int getHorizontalPosition();
    void setHorizontalPosition(int hPos);
    int updateHorizontalPosition(int hPos, int hCurrentPos = -1);
    int getVerticalPosition();
    void setVerticalPosition(int vPos);
    int updateVerticalPosition(int vPos, int vCurrentPos = -1);
    bool syncWith(PedestalHandler master);

  private:
    int count;
#if !__HCPCA9685_ENABLED__
    Servo horizontalServo;
#endif
    byte horizontalServoPin;
    int horizontalServoPos;
    int horizontalMinAngle;
    int horizontalMaxAngle;
#if !__HCPCA9685_ENABLED__
    Servo verticalServo;
#endif
    byte verticalServoPin;
    int verticalServoPos;
    int verticalMinAngle;
    int verticalMaxAngle;
    bool debugEnabled;
};

#endif

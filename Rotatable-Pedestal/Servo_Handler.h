#include <Servo.h>

#ifndef __SERVOS_HANDLER_H__
#define __SERVOS_HANDLER_H__

#define HORIZONTAL_SERVO_PIN 8
#define VERTICAL_SERVO_PIN 9

static Servo horizontalServo;
static Servo verticalServo;

void initServos();
void testServos();

bool verticalServoUp();
bool verticalServoDown();
bool horizontalServoLeft();
bool horizontalServoRight();

#endif

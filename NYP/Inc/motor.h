#ifndef __MOTOR_H__
#define __MOTOR_H__

void MotorLeftDuty(float duty);
void MotorRightDuty(float duty);

// Shell interface function
void cmd_motor(int argc, char *argv[]);

#endif // __MOTOR_H__

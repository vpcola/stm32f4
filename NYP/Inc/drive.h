#ifndef __DRIVE_H__
#define __DRIVE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#define DEFAULT_KP 0.5f  // Proportional
#define DEFAULT_KI 0.03f // Integral
#define DEFAULT_KD 0.02f // Differential   

extern osSemaphoreId driveUpdaterHandle;

typedef struct 
{
    int pwmLeft;
    int pwmRight;

    volatile int countLeft, oldCountLeft;
    volatile int countRight, oldCountRight;

    int tmcount;    // TIM7 interrupts every 1 sec, we inrement this var.

    int rpmLeft;    // calculated rpm based on pwm
    int rpmRight;   // calculated rpm based on pwm
    int actRpmLeft; // measured rpm based on sensor
    int actRpmRight; // measured rpm based on sensor
} DriveParameters;    

typedef struct{
    volatile float error;
    volatile float old_error;
    volatile float sum;
    volatile float kp;
    volatile float ki;
    volatile float kd;
    volatile float control;
} PIDControl;

void initDriveSemaphores(void);
/* Call to create worker thread */
void startDriveUpdaterTask(void);
/* Worker thread, call updateDriveParams */
void driveUpdaterTask(void const * argument);
/* This function is continually called from
 * a worker thread to update the drive parameters
 * based on sensor readings
 */
void updateDriveParams(void);
void InitPIDParams(PIDControl * c);
void CalcPID(PIDControl * c, float e);


#ifdef __cplusplus
}
#endif

#endif


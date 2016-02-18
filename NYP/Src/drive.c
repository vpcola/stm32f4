#include "drive.h"
#include "tim.h"
#include "motor.h"
#include "gpio.h"


osSemaphoreId driveUpdaterHandle;
//static osThreadId driveUpdaterTaskHandle;
static DriveParameters driveParams;
static PIDControl pidLeft, pidRight;
static int recurrencePID;
static float pidArray_L;
static float pidArray_R;

void driveUpdaterTask(void const * argument);

void initDriveSemaphores(void)
{
    osSemaphoreDef(driveUpdaterSemaphore);
    driveUpdaterHandle = osSemaphoreCreate(osSemaphore(driveUpdaterSemaphore), 1);
}

void updateDriveParams(void)
{
    float rtvelocity_L = 0.0;
    float rtvelocity_R = 0.0;
    // TODO: 
    // Reads encoder values and convert them into rpm speed.
    // Records the error, prime the PIDs
    // and send the resulting PWMs to command the motor

    // Calculate desired RPM by the input pwm.
    // Based on motor parameters, 200rpm free running speed at 6V (no load).
    // Since pwm is directly proportional to pwm volatage, it also means that
    // 50% pwm = 100 rpm
    // 60% pwm = 120 rpm
    // 70% pwm = 140 rpm
    // ....
    // 100% pwm = 200 rpm
    //
    // pwm = 2 * rpm;
    // or rpm = pwm / .5
    //
    driveParams.rpmLeft = driveParams.pwmLeft / 0.5 ; 
    driveParams.rpmRight = driveParams.pwmRight / 0.5 ;

    // Now we calculate the actual RPMs based on motor
    // rotation from the quadrature sensors
    // Current RPM = (finalleft - countleft) * 60 (revs/min)
    // Our slow motor has a gear ratio of 47:1 and provides 2248.86 counts per revolution.
    // Since this function updateDriveParams is called every 1 second, we multiply by 60
    // to get revolutions / minute.
    driveParams.actRpmLeft = ((driveParams.countLeft - driveParams.oldCountLeft) / 2248.86) * 60;
    driveParams.actRpmRight = ((driveParams.countRight - driveParams.oldCountRight) / 2248.86) * 60;

    // Now that we have the desired and acutal rpm values,
    // enter these into our PID controller to get the desired pwm values
    // we send to the motor.
    CalcPID(&pidLeft, driveParams.actRpmLeft - driveParams.rpmLeft);
    // Corrected motor rpm is in .control
    pidArray_L += pidLeft.control;

    CalcPID(&pidRight, driveParams.actRpmRight - driveParams.rpmRight);
    // Corrected motor rpm is in .control
    pidArray_R += pidRight.control;

    recurrencePID++;
    if (recurrencePID >= 10)
    {
        rtvelocity_L += pidArray_L / recurrencePID;
        rtvelocity_R -= pidArray_R / recurrencePID;
        recurrencePID = 0;
        pidArray_L = 0.0;
        pidArray_R = 0.0;

        // Finally set the motor pwms (note pwm = 2 * rpm)
        //
        //MotorLeftDuty( rtvelocity_L * 2 );
        //MotorRightDuty( rtvelocity_R * 2 );
    }

}

void startDriveUpdaterTask(void)
{
     // Initialize driveParams
     driveParams.rpmLeft = 0;
     driveParams.rpmRight = 0;
     driveParams.oldCountLeft = 0;
     driveParams.oldCountRight = 0;

     recurrencePID = 0;

     // Initialize PID constants
     InitPIDParams(&pidLeft);
     InitPIDParams(&pidRight);

     //osThreadDef(driveUpdate, driveUpdaterTask, osPriorityNormal, 0, 128);
     //driveUpdaterTaskHandle = osThreadCreate(osThread(driveUpdate), NULL);

     /* Enable TIM7 to start generating interrupts */
     //if (driveUpdaterTaskHandle)
     //    TIM7_Start();
}

void driveUpdaterTask(void const * argument)
{
    while(1)
    {
        if (xSemaphoreTake(driveUpdaterHandle, portMAX_DELAY) == pdTRUE)
        {
            // Update drive parameters here
            updateDriveParams();
        }
    }
}

/* This routine is called whenever TIM7's count overflows */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // Check if we are catering to HTIM7
    if (htim->Instance == TIM7)
    {
        // record the left/right counts here
        driveParams.countLeft =  TIM2_GetCount();
        // TODO:
        // TIM4 is a 16-bit counter, it may always overflow!
        // But since our motor is sooo slow, we can ignore this 
        // for now.
        driveParams.countRight = TIM4_GetCount();
        driveParams.tmcount++;

			  Led1P_Toggle();
			
        /* Let the driveUpdater task finish the 
         * computing intensive routines for motor PID  */
        //xSemaphoreGiveFromISR(driveUpdaterHandle, NULL);
    }
}

void InitPIDParams(PIDControl * c)
{
    c->kp = DEFAULT_KP;
    c->ki = DEFAULT_KI;
    c->kd = DEFAULT_KD;
}

void CalcPID(PIDControl * c, float e)
{
    c->old_error = c->error;
    c->error = e;
    c->sum += 0.001f * e;

    c->control = c->kp * c->error + c->kd * (c->error - c->old_error) + c->ki * c->sum;
}


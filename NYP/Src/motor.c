#include "motor.h"
#include "tim.h"
#include "shell.h"
#include "cmsis_os.h" 

#include <stdlib.h>
#include <string.h>

void MotorLeftDuty(float duty)
{
	// Left motor is controlled by Channel 1 & 2 of TIM3
	// PA6     ------> TIM3_CH1
  // PA7     ------> TIM3_CH2
	// To turn the motor forward, feed channel 1 with positive pwm
	// while channel 2 remains at zero.
	// To turn the motor backwards, feed channel 2 with positive pwm
	// while channel 1 remains at zero.
	if (duty > 0) // forward motion
	{
		TIM3_SetDuty(TIM_CHANNEL_1, duty);
		TIM3_SetDuty(TIM_CHANNEL_2, 0);
	}
	else if (duty < 0)
	{
		TIM3_SetDuty(TIM_CHANNEL_1, 0);
		TIM3_SetDuty(TIM_CHANNEL_2, abs(duty));
	}else // Stop, both channels zeroed
	{
		TIM3_SetDuty(TIM_CHANNEL_1, 0);
		TIM3_SetDuty(TIM_CHANNEL_2, 0);
	}
	
	TIM3_StartPWM(TIM_CHANNEL_1);
	TIM3_StartPWM(TIM_CHANNEL_2);		
}

void MotorRightDuty(float duty)
{
	// Right motor is controlled by Channel 3 & 4 of TIM3
  // PB0     ------> TIM3_CH3
  // PB1     ------> TIM3_CH4 
	// To turn the motor forward, feed channel 3 with positive pwm
	// while channel 4 remains at zero.
	// To turn the motor backwards, feed channel 3 with positive pwm
	// while channel 4 remains at zero.
	if (duty > 0) // forward motion
	{
		TIM3_SetDuty(TIM_CHANNEL_3, duty);
		TIM3_SetDuty(TIM_CHANNEL_4, 0);
	}
	else if (duty < 0)
	{
		TIM3_SetDuty(TIM_CHANNEL_3, 0);
		TIM3_SetDuty(TIM_CHANNEL_4, abs(duty));
	}else // Stop, both channels zeroed
	{
		TIM3_SetDuty(TIM_CHANNEL_3, 0);
		TIM3_SetDuty(TIM_CHANNEL_4, 0);
	}
	
	TIM3_StartPWM(TIM_CHANNEL_3);
	TIM3_StartPWM(TIM_CHANNEL_4);		
}

void cmd_motor(int argc, char *argv[])
{
  (void)argv;
	float dutyval = 0.0;
	int leftorright = 0;
	
  if (argc != 2 ) 
  {
    shellUsage("motor <left/right> <duty>");
    return;
  }
	
	// Parse which motor to set.
	if (strcmp(argv[0], "left") == 0)
	{
		leftorright = 0;
	}
	else if(strcmp(argv[0], "right") == 0)
	{
		leftorright = 1;
	}
	else
	{
		shellUsage("motor <left/right> <duty>");
		return;
	}
	
	if (strcmp(argv[1], "on") == 0)
	{
		if (leftorright == 0) // Left motor
		{
			// turn on left motor
			TIM3_StartPWM(TIM_CHANNEL_1);
			TIM3_StartPWM(TIM_CHANNEL_2);
		}else
		{
			// turn on right motor
			TIM3_StartPWM(TIM_CHANNEL_3);
			TIM3_StartPWM(TIM_CHANNEL_4);
		}
		return;
	}
	
	if (strcmp(argv[1], "off") == 0)
	{
		if (leftorright == 0) // Left motor
		{
			// turn off left motor
			TIM3_StopPWM(TIM_CHANNEL_1);
			TIM3_StopPWM(TIM_CHANNEL_2);
		}else
		{
			// turn off right motor
			TIM3_StopPWM(TIM_CHANNEL_3);
			TIM3_StopPWM(TIM_CHANNEL_4);
		}	
		return;
	}
	
  // If we get here, we extract the duty cycle
  // from the second argument.	
	dutyval = atof(argv[1]);
	
	// And set the appropriate PWM for the motors
	if (leftorright == 0)
	{
		MotorLeftDuty(dutyval);
	}else
	{
		MotorRightDuty(dutyval);
	}
	
}

void cmd_encoder(int argc, char *argv[])
{
	(void)argv;
	int countleft = 0;
	int countright = 0;
	int finalleft;
	//int finalright;
	int rpmleft;
	//int rpmright;
	int speed;
	
	if ((argc == 1) && (strcmp(argv[0], "clear") == 0))
	{
		TIM2_ResetCount();
		TIM4_ResetCount();
		return;
	}
	
	countleft = TIM2_GetCount();
	countright = TIM4_GetCount();
	
	printf("Left Encoder : %d\r\n", countleft);
	printf("Right Encoder : %d\r\n", countright);
	// Display number of wheel turns
	// Gear ratio is 47:1, 48 CPR encoder yeilding 2248.86 counts/revolution
	printf("Number of turns (left) : %d\r\n", (int) (countleft/2248.86) );
	printf("Number of turns (right) : %d\r\n", (int) (countright/2248.86) );
	
	printf("Calculating speed ....\r\n");
	countleft = TIM2_GetCount();
	countright = TIM4_GetCount();
	osDelay(1000); // Delay for 1 second
	finalleft = TIM2_GetCount();
	//finalright = TIM4_GetCount();
	// Current RPM = (finalleft - countleft) * 60 (revs/min)
	rpmleft = ((finalleft - countleft) / 2248.86) * 60;
	printf("RPM for left motor = %d\r\n", rpmleft);
	//rpmright = ((finalright - countright) / 2248.86) * 60;
	// Speed = (RPM (diameter * PI) / 60)
	speed = rpmleft * (2.5 * 3.1415) / 60;
	printf("Vehicle speed = %d inches/second\r\n", speed);
}


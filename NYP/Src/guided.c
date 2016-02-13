#include "guided.h"
#include "cmsis_os.h"
#include "motor.h"
#include "adc.h"
#include "shell.h"
#include "usart.h"

#include <math.h>
#include <stdio.h>

static MotorDuty motorDuty;

MotorDuty * getMotorDuty(int x, int y)
{
	float xfactor;
	float yfactor;
  // int midpoint = 1280; // Middle value of x and y

  // Get left and right to move forward
  // x and y retains equal magnitude
	if ( x > 1280 )
	{
		xfactor = 28.16; // (4096 (Xmax) - 1280) / 100 Step counts
		
		// compute relative magnitude from 1280 to 4096
    motorDuty.lduty = (int) (( x - 1280 ) / xfactor);
    motorDuty.rduty = motorDuty.lduty;
		

	}else if ( x < 1280 )
	{
		xfactor = 9.8 ; // (1280 - Xmin (300)) / 100 
		motorDuty.lduty = (int)(( 1280 - x ) / xfactor) * -1; 
		motorDuty.rduty = motorDuty.lduty;
	}

  // Based on the value of y, we damp
  // existing values above to move the motor
  // left or right.
  if ( y > 1280 )
  {
		yfactor = 28.16;
     // Move towards right, increase motor left
     // duty and decrease right motor duty fac.
     motorDuty.lduty -= (y - 1280) / yfactor;
     motorDuty.rduty += (y - 1280) / yfactor;
  } else if ( y < 1280 )
  {
		yfactor = 9.6;
		
     // Move towards left, increase motor right
     // duty and decrease left motor duty fac.
     motorDuty.lduty += (1280 - y) / yfactor;
     motorDuty.rduty -= (1280 - y) / yfactor;
  }
	  printf("Motor right duty : %d     \r\n", motorDuty.rduty);
	  printf("Motor left duty  : %d     \r\n", motorDuty.lduty);
    return &motorDuty;
}



void cmd_runguided(int argc, char *argv[]) 
{
    (void)argv;
    int x, y;
    printf("Running in guided mode!\r\n");
    printf("Press any key to exit ...\r\n");

    while(!Usart_HasData())
    {
       // Read current joystick values 
       x = readJoystickX();
       y = readJoystickY();
			 	
	     printf("X = %d, Y = %d   \r\n", x, y);
       getMotorDuty(x, y); // Gets the duty cycle of the motors
                        // from the current x and y values of the joystick
			 printf("Duty left = %03d, Duty right = %03d   \r\n", motorDuty.lduty, motorDuty.rduty);
       // Send the Duty cycle values to the motor
       MotorLeftDuty((float) motorDuty.lduty);
       MotorRightDuty((float) motorDuty.rduty);
			 shellGoBackLine(4); 
       osDelay(100); // Delay for 100 MS 
    }
		
		shellClearScreen();
}



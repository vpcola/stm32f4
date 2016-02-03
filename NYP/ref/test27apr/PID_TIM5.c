#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "stm32f4xx.h"
#include "device.h"
#include "PID_TIM5.h"
#include "device.h"

volatile int enable_con_wall = 1;
float tm_speed_l,tm_speed_r,rm_speed_l=50,rm_speed_r=50,TotalArray_l=0,TotalArray_r=0;
int velocity_r,velocity_l;
int RecurrencePID = 0;

void InitializeTimer5()
{
    TIM_TimeBaseInitTypeDef timerInitStructure;
    // period 1000 == 1Khz,1000us
    //prescaller 42 == 1Khz, 1ms
    timerInitStructure.TIM_Prescaler =42;
    timerInitStructure.TIM_Period =1000-1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &timerInitStructure);
    TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

void EnableTimerInterrupt_TIM5()
{
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM5_IRQn;		//Activate the timer interrupt 5
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

void TIM5_IRQHandler()
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		button1J = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	    button2L = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
	    button3M = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
	    button4goL= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
	    button5goR = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);


	    if(button5goR == 1){

				if(button1J == 1)		//the speed fast or low
				{
					if (button3M == 1)
						Controller(2000,4200);

					else if (button4goL == 1)
						Controller(4200,2000);

					else
						Controller(2000,2000);
				}

	    		else if(button2L == 1)
	    				Controller(-2000,-2000);

	    		else if(button3M == 1)
	    				Controller(-1500,1500);

	    		else if(button4goL == 1)
	    				Controller(1500,-1500);

	    		else {		//else stop the wheels
	    			setLeftPWM3(0);
	    			setRightPWM3(0);
	    			RTvelocity_l = RTvelocity_r = 0;
	    		}
	    }

	    else if(button5goR == 0){

				if(button1J == 1){		//the speed fast or low
					if(button3M == 1)
						Controller(3200,7000);

					else if(button4goL == 1)		//turn right while moving forward (yes at right...)
						Controller(7000,3200);

					else
						Controller(6000,6000);
				}

	    		else if(button2L == 1)
	    				Controller(-6000,-6000);

	    		else if(button3M == 1)
	    				Controller(-5500,5500);

	    		else if(button4goL == 1)
	    				Controller(5500,-5500);

	    		else {		//else stop the wheels
	    			setLeftPWM3(0);
	    			setRightPWM3(0);
	    			RTvelocity_l = RTvelocity_r = 0;
	    		}
	    }




        /*
         *         if(button3M == 0){		//if the button is on, do the PID
        	if(button2L == 1)		//the speed fast or low
        			Controller(1500,1500);
        	else
        			Controller(3500,3500);
        }

        else if (button4goL == 0) {		//If we want to go Left
        	if(button1J == 1)			//depend of the direction of the robot
        		Controller(-1000,-2500);
        	else
        		Controller(-2500,-1000);
        }

        else if (button5goR == 0) {		//If we want to go Right
        	if(button1J == 1)
        		Controller(2500,1000);
        	else
        		Controller(1000,2500);
        }

        else {		//else stop the wheels
         	setLeftPWM3(0);
         	setRightPWM3(0);
         	RTvelocity_l = RTvelocity_r = 0;
		}
         */

    }
}

void Controller(int pwm_speed_l,int pwm_speed_r){
    update_machine();
	con_spd_l.kp = 0.5f;     //was 0.5
    con_spd_l.kd = 0.02f;	//was 0.02
    con_spd_l.ki = 0.03f;     //was 0.03

	con_spd_r.kp = 0.5f;     //was 0.5
    con_spd_r.kd = 0.02f;	//was 0.02
    con_spd_r.ki = 0.03f;     //was 0.03

    tm_speed_l = (pwm_speed_l*117)/8399;		//convert pwm cycle in count
    tm_speed_r = (pwm_speed_r*117)/8399;
    regulate_machine_speed();

    /*
	if(button1J == 1){			//if the button is off, we moove forward( == left wheel is negative, right is positive)
		//Maximum speed is 116 counting per ms at 1KHz frequency
	    tm_speed_l = (pwm_speed_l*117)/8399;		//convert pwm cycle in count
	    tm_speed_r = (pwm_speed_r*117)/8399;
	    regulate_machine_speed();
	}
	else{						//if the button is on, we moove backward
	    tm_speed_l = -(pwm_speed_l*117)/8399;		//convert pwm cycle in count
	    tm_speed_r = -(pwm_speed_r*117)/8399;
	    regulate_machine_speed();
	}
	*/
}

void update_machine(void){
	rm_speed_l = difTIM2;		//convert count in pwm cycle values
    rm_speed_r = difTIM4;
}

void regulate_machine_speed(void){
	//___________FOR PID MOTOR LEFT__________________
	calc_PID(&con_spd_l, tm_speed_l - rm_speed_l);
	TotalArray_l += con_spd_l.control;		//regulation of the speed

	//___________FOR PID MOTOR RIGHT__________________
	calc_PID(&con_spd_r, tm_speed_r - rm_speed_r);
	TotalArray_r += con_spd_r.control;		//regulation of the speed

	RecurrencePID++;
    if(RecurrencePID>=10){		//take the average of ten sample
    	RTvelocity_l +=  TotalArray_l/RecurrencePID;
    	RTvelocity_r -=  TotalArray_r/RecurrencePID;	//it has to be negative, so the right wheel is moving forward
    	RecurrencePID =TotalArray_l = TotalArray_r = 0;	//re-initialise the counter
		setLeftPWM3(RTvelocity_l);
		setRightPWM3(RTvelocity_r);
    }
}

void calc_PID(t_control *c,float e){

    c->old_error = c->error;
    c->error = e;
    c->sum += 0.001f * e;

    c->control = c->kp * c->error + c->kd * (c->error - c->old_error) + c->ki * c->sum;
}

#include "stm32f4xx.h"
#include "device.h"

void PWM3_Configuration()
{
	  //_________________________ Init leds _________________________________________
	    GPIO_InitTypeDef GPIO_InitStruct;

	        /* Clock for GPIOD */
	        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	        /* Alternating functions for pins */
	    	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	    	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
	    	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	    	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

	        /* Set pins */
	    	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	    	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	    	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	    	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	    	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    	GPIO_Init(GPIOA, &GPIO_InitStruct);

	    	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	    	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	    	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	    	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	    	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    	GPIO_Init(GPIOB, &GPIO_InitStruct);

		//___________________________Init timer_______________________________
		   TIM_TimeBaseInitTypeDef TIM_BaseStruct;

		   /* Enable clock for TIM3 */
		   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	   /*
		   TIM3 is connected to APB1 bus, which has on F407 device 42MHz clock
		   But, timer has internal PLL, which double this frequency for timer, up to 84MHz
		   Remember: Not each timer is connected to APB1, there are also timers connected
		   on APB2, which works at 84MHz by default, and internal PLL increase
		   this to up to 168MHz

		   Set timer prescaller
		   Timer count frequency is set with

		   timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)

		   In our case, we want a max frequency for timer, so we set prescaller to 0
		   And our timer will have tick frequency

		   timer_tick_frequency = 84000000 / (0 + 1) = 84000000
	   */
		   TIM_BaseStruct.TIM_Prescaler = 1;
		   /* Count up */
		   TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	   /*
		   Set timer period when it have reset
		   First you have to know max value for timer
		   In our case it is 16bit = 65535
		   To get your frequency for PWM, equation is simple

		   PWM_frequency = timer_tick_frequency / (TIM_Period + 1)

		   If you know your PWM frequency you want to have timer period set correct

		   TIM_Period = timer_tick_frequency / PWM_frequency - 1

		   In our case, for 10Khz PWM_frequency, set Period to

		   TIM_Period = 84000000 / 10000 - 1 = 8399

		   If you get TIM_Period larger than max timer value (in our case 65535),
		   you have to choose larger prescaler and slow down timer tick frequency
	   */
		   TIM_BaseStruct.TIM_Period = 8399; /* 10kHz PWM */
		   TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
		   TIM_BaseStruct.TIM_RepetitionCounter = 0;
		   /* Initialize TIM3 */
		   TIM_TimeBaseInit(TIM3, &TIM_BaseStruct);
		   /* Start count on TIM3 */
		   TIM_Cmd(TIM3, ENABLE);


	    //___________________________ Init PWM ___________________________
		   TIM_OCInitTypeDef TIM_OCStruct;
		   // PWM mode 2 = Clear on compare match
		   TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		   TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		   TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	   /*
		   To get proper duty cycle, you have simple equation

		   pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1

		   where DutyCycle is in percent, between 0 and 100%

		   25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
		   50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
		   75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
		   100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399

		   Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
	   */
		   TIM_OCStruct.TIM_Pulse = -4199; /* 25% duty cycle */
		   TIM_OC1Init(TIM3, &TIM_OCStruct);
		   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

		   TIM_OCStruct.TIM_Pulse = 4199; /* 50% duty cycle */
		   TIM_OC2Init(TIM3, &TIM_OCStruct);
		   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

		   TIM_OCStruct.TIM_Pulse = 6299; /* 75% duty cycle */
		   TIM_OC3Init(TIM3, &TIM_OCStruct);
		   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	       TIM_OCStruct.TIM_Pulse = 8399; // 100% duty cycle
		   TIM_OC4Init(TIM3, &TIM_OCStruct);
		   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

void setLeftPWM3(int16_t pwm) {
  if (pwm == 0) {
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 0;
  }
  else if (pwm > 0) {

    TIM3->CCR1 = pwm;
    TIM3->CCR2 = 0;
  }
  else if (pwm < 0) {
    TIM3->CCR1 = 0;
    TIM3->CCR2 = -pwm;

  }
}

void setRightPWM3(int16_t pwm) {
	if (pwm == 0) {
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 0;
	}
  else if (pwm > 0) {
    TIM3->CCR3 = pwm;
    TIM3->CCR4 = 0;
  	  }
  else if (pwm < 0) {
    TIM3->CCR3 = 0;
    TIM3->CCR4 = -pwm;
  }
}

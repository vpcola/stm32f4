#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "stm32f4xx.h"
#include "device.h"

void InitializeTimer7()
{
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 42;// 672;// 168 MHz Clock down to 1 MHz (adjust per your clock) 168/8
    timerInitStructure.TIM_Period = 1000-1;//62500-1;	// 1 MHz Clock down to 1 KHz
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM7, &timerInitStructure);
    TIM_Cmd(TIM7, ENABLE);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

void EnableTimerInterrupt_TIM7()
{
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM7_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

void TIM7_IRQHandler()
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        GPIO_ToggleBits(GPIOB,GPIO_Pin_4);
		//____________________________Read the motor Encoder____________________________

        //__________TIM2 32bits__________
		newTIM2 = ReadMotorEncoder(0);
		difTIM2 = newTIM2 - oldTIM2;
		oldTIM2 = newTIM2;

		//__________TIM4 16bits__________
		newTIM4 = ReadMotorEncoder(1);
		difTIM4 = newTIM4-oldTIM4;
		if(difTIM4<-30000)	//Wheel move forward
			difTIM4 = newTIM4 + (65535-oldTIM4);
		if(difTIM4>30000)	//Wheel move backward
			difTIM4 = oldTIM4 + (65535 - newTIM4);
		oldTIM4 = newTIM4;

		//__________Position robot__________
        int DWheelCm = 50;//2*3.14*(15.24/2);		//~50cm
        int CountPerRevolution = 58867;

		CountingLeft += difTIM2;
		CountingRight +=  difTIM4;
		positionLeft  = ((CountingLeft)/CountPerRevolution)*DWheelCm;
		positionRight  = ((CountingRight)/CountPerRevolution)*DWheelCm;


    }
}

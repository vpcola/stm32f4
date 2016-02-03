/**
  ******************************************************************************
  * @file    Pulse Width Modulation Demo
  * @author  Lee Dyche - Tecsploit.com
  * @version V1.0.0
  * @date    18/03/2014
  * @brief   3 LEDS go light to dim, and the GPIO pin PB6 has its voltage ramped up and down
  * and could be used to control the brightness of a LED / power of a motor
  ******************************************************************************
  * @attention
  * This is program is provided as is with no warranty!!
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "device.h"

void PWM_LED_Configuration()
{
	/*Structures used in the configuration*/
  TIM_TimeBaseInitTypeDef  timerInitStructure;
  TIM_OCInitTypeDef  outputChannelInit;
  GPIO_InitTypeDef gpioStructure;

 // void InitializeTimer(int period = 500)
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

      timerInitStructure.TIM_Prescaler = 40000;
      timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
      timerInitStructure.TIM_Period = 500;
      timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
      timerInitStructure.TIM_RepetitionCounter = 0;
      TIM_TimeBaseInit(TIM4, &timerInitStructure);
      TIM_Cmd(TIM4, ENABLE);


 //InitializePWMChannel()
      outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
      outputChannelInit.TIM_Pulse = 400;
      outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
      outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

      TIM_OC1Init(TIM4, &outputChannelInit);
      TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

      GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_TIM4);


 //InitializeLEDs
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

      gpioStructure.GPIO_Pin = GPIO_Pin_13;
      gpioStructure.GPIO_Mode = GPIO_Mode_AF;
      gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOD, &gpioStructure);
}

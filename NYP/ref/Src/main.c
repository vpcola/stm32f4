
#include "stm32f4xx.h"
#include "device.h"
#include <stdio.h>

void Robot_Configuration(void);

int main(void) {
	uint16_t sensor[8];
	Robot_Configuration();

	ResetButtonStatusFlag();
    while(1)
    {
    	ReadSensor(sensor);
    	printf(" %04d %04d %04d %04d %04d %04d %04d %04d "
    			"%6d %6d\n", sensor[0], sensor[1], sensor[2], sensor[3], sensor[4], sensor[5], sensor[6], sensor[7],
    			ReadMotorEncoder(0), ReadMotorEncoder(1));
    	delay(200);

    	if (Button_SW1()) {
    		TurnOnLEDs(0x0f);
			activateMotor();
			setLeftPWM(100);
			//setLeftPWM(-100);
			delay(1000);
			setLeftPWM(0);
			deactivateMotor();
		}
    	if (Button_SW2()) {
    		TurnOnLEDs(0xf0);
    		activateMotor();
    		setRightPWM(100);
    		//setRightPWM(-100);
    		delay(1000);
    		setRightPWM(0);
    		deactivateMotor();
    	}
    	TurnOnLEDs(0);
    }
}

void Robot_Configuration(void) {
	SystemInit();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);
	}

	NVIC_Configuration();
	UART_Configuration();
	LED_Configuration();
	Button_Configuration();
	Buzzer_Configuration();
	Encoder_Configuration();
	Sensor_Configuration();
	Motor_Configuration();

	delay(200);
}

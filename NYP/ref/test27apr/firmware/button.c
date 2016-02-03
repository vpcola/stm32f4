
#include "stm32f4xx.h"
#include "device.h"

volatile uint8_t lButtonStatusFlag = FALSE, rButtonStatusFlag = FALSE;
volatile uint8_t lButtonCycle = 0, rButtonCycle = 0;

static void softDelay(void) {
	uint32_t i;
	for(i = 0; i < 0xffff; i++);
}

void Button_Configuration(void) {
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);

	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_ClearITPendingBit(EXTI_Line12);
	EXTI_ClearITPendingBit(EXTI_Line13);

	ResetButtonStatusFlag();
}

void EXTI15_10_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line12);
		softDelay();
		if (!(GPIOC->IDR & GPIO_Pin_8)) rButtonStatusFlag = 1;
		else rButtonStatusFlag = 0;
	}

	if(EXTI_GetITStatus(EXTI_Line13) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line13);
		softDelay();
		if (!(GPIOC->IDR & GPIO_Pin_9)) lButtonStatusFlag = 1;
		else lButtonStatusFlag = 0;
	}
}

int8_t Button_SW1(void) {
	if(rButtonStatusFlag == TRUE) {
		while((GPIOC->IDR & GPIO_Pin_8) == GPIO_Pin_8) asm volatile ("nop");
		rButtonStatusFlag = FALSE;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

int8_t Button_SW2(void) {
	if(lButtonStatusFlag == TRUE) {
		while((GPIOC->IDR & GPIO_Pin_9) == GPIO_Pin_9) asm volatile ("nop");
		lButtonStatusFlag = FALSE;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void ResetButtonStatusFlag(void) {
	lButtonStatusFlag = rButtonStatusFlag = FALSE;
}

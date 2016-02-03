
#include "stm32f4xx.h"
#include "device.h"

void LED_Configuration(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA, GPIO_Pin_12 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12, Bit_RESET);
}

void TurnOnLEDs(int8_t LEDs) {
	LED1_ON((LEDs & 0x01) == 0x01);
	LED2_ON((LEDs & 0x02) == 0x02);
	LED3_ON((LEDs & 0x04) == 0x04);
	LED4_ON((LEDs & 0x08) == 0x08);
	LED5_ON((LEDs & 0x10) == 0x10);
	LED6_ON((LEDs & 0x20) == 0x20);
	LED7_ON((LEDs & 0x40) == 0x40);
	LED8_ON((LEDs & 0x80) == 0x80);
}

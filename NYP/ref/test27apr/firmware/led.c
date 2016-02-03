
#include "stm32f4xx.h"
#include "device.h"

void LED_Configuration(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 , Bit_SET);

	delay(200);
	AllLedON();
//	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_RESET);
// 	GPIO_WriteBit(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, Bit_RESET);


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

void ToggleLEDs() {
	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, Bit_SET);

	delay(200);

	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, Bit_RESET);


}

void AllLedON() {
	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, Bit_SET);



}

void AllLedOFF() {
	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, Bit_RESET);

}

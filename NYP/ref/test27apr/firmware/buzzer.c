
#include "stm32f4xx.h"
#include "device.h"

void Buzzer_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)(0));
}

void BuzzerSound(int8_t x) {
	uint16_t i, j;

	if (x > 5) x = 5;
	for (i = 0; i < (225 - (x*35)); i++) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET); for (j = 0; j < 3000 + (x*25); j++);
		GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET); for (j = 0; j < 3000 + (x*25); j++);
	}
}

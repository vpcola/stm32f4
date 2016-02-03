
#include "stm32f4xx.h"
#include "device.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void UART_Configuration(void) {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	//interuption
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);


	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	volatile char test[30+1]= {"bonjour"};
	USART_write(USART1, test);
}


void USART1_IRQHandler(void){

#define MAX_STRLEN 120 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1];

// check if the USART1 receive interrupt flag was set
	if (USART_GetITStatus(USART1,USART_IT_RXNE)) {
		static uint8_t cnt = 0; // this counter is used to determine the string length
		char t = USART1->DR; // the character from the USART1 data register is saved in t

		// check if the received character is not the LF character (used to determine end of string)
		// or the if the maximum string length has been been reached

		if( (t != 'q') && (cnt < MAX_STRLEN) ){
			received_string[cnt] = t;
			cnt++;
		}
		else{ // otherwise reset the character counter and print the received string
			cnt = 0;
			//int HexaReceiving = 0x00;
			//atoh(received_string);
			USART_write(USART1, received_string);

			int msbPayload,lsbPayload;
			if (received_string[4]>57)
				msbPayload = received_string[4]-55;
			else
				msbPayload = received_string[4]-48;

			if (received_string[5]>57)
				lsbPayload = received_string[5]-55;
			else
				lsbPayload = received_string[5]-48;

			char PayloadLength=0;
			PayloadLength = 10*msbPayload + lsbPayload;		//take the two numbers as a decimal
			printf("The payload lenght is %d. \n\r",PayloadLength);
		//	int speed =
		//	Control_wheel_motor();
		}

	}
}


void USART_write(USART_TypeDef* USARTx, volatile char *s){
		do
		{
		 while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);  //wait to be empty
		 USART_SendData(USARTx, *s++);
		}while(*s);
}




/*void USART1_IRQHandler(void){

#define MAX_STRLEN 120 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1];

	GPIO_WriteBit(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_6 | GPIO_Pin_7, Bit_RESET);

	delay(500);

		// check if the USART1 receive interrupt flag was set
		if( USART_GetITStatus(USART1, USART_IT_RXNE) ){

			static uint8_t cnt = 0; // this counter is used to determine the string length
			char t = USART1->DR; // the character from the USART1 data register is saved in t

			// check if the received character is not the LF character (used to determine end of string)
			// or the if the maximum string length has been been reached

			if( (t != '\n') && (cnt < MAX_STRLEN) ){
				received_string[cnt] = t;
				cnt++;
			}
			else{ // otherwise reset the character counter and print the received string
				cnt = 0;
				USART_write(USART1, received_string);
			}
		}
}
*/




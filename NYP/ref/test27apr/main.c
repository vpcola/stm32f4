#include "stm32f4xx.h"
#include "device.h"
#include <stdio.h>
#include <string.h>

void Robot_Configuration(void);


int main(void) {
	oldTIM2=newTIM2=difTIM2=oldTIM4=newTIM4=difTIM4=CountingRight=CountingLeft=positionRight=positionLeft=0;

	RTvelocity_r = 0.0f;
	RTvelocity_l = 0.0f;

	//uint16_t sensor[8];
  	Robot_Configuration();
	ResetButtonStatusFlag();

	setLeftPWM3(1000);
	setRightPWM3(1000);
    while(1)
    {
    	printf("\n\r     Left Speed %d, Counter %d, PWM Value %d",difTIM2,CountingLeft,(int)RTvelocity_l);
    	printf("\n\rRight Speed %d, Counter %d, PWM Value %d",difTIM4,CountingRight,(int)RTvelocity_r);

		//____________________________Receive Inputs states____________________________
    		//define in device.h

    		button1J = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
		    button2L = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
		    button3M = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
		    button4goL= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
		    button5goR = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
			//test6 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
			printf("PIN : %d%d%d%d%d.\n\r",button1J,button2L,button3M,button4goL,button5goR);//test6);

    }
}

void Robot_Configuration(void) {
	SystemInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7|RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

	if (SysTick_Config(SystemCoreClock / 1000)) {
	}

	NVIC_Configuration();
	UART_Configuration();
	LED_Configuration();
	//Button_Configuration();
	Buzzer_Configuration();

	Sensor_Configuration();
	Motor_Configuration();
	PWM3_Configuration();
	Encoder_Configuration();

	//_____TIMERS________
	InitializeTimer7();
	EnableTimerInterrupt_TIM7();

	InitializeTimer5();
	EnableTimerInterrupt_TIM5();
	inputMaleHeader_Configuration();			//disable pwm GPIO6/7 is activated

	delay(200);
}



//____________________________UART_____________________________________________
/*  	ToggleLEDs();
	char str[] = "Init USART finished ! \n\r ";
	USART_write(USART1, str);
	//char * dataReceived = malloc(i * sizeof(char));
	char dataReceived[1000];
	int i=0, j=0;

	printf(" Data to Send :");
	do{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
		dataReceived[i]=  USART1->DR;
		i++;
	}while(dataReceived[i-1]!=13);  //13==\n
	dataReceived[i]='\0';

	printf("\n\rYour sentence is : %s\n\n\r",dataReceived);
*/

//receiving bytestream
/*		char dataReceived[400];
	int i=0, j=0;
	ToggleLEDs();
	printf("Let's go,");
	delay(200);

	do{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
	}while(USART1->DR != 0xAA);		//Wait to have a bytestream
	printf("The Bytestream :");
   	printf("AA ");
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);

	do{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
	}while(USART1->DR != 0x55);		//Wait to have a bytestream
	printf("55 ");
	GPIO_WriteBit(GPIOB, GPIO_Pin_14 , Bit_SET);
*/




	//Get back the ten first bytes
	/*GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
	AllLedON();
	delay(500);*/

/*
	do{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
		dataReceived[i]= USART_ReceiveData(USART1);//USART1->DR;
		//printf("%x ",dataReceived[i] & 0xFF);	//display the hexa data received
		i++;
		//SwitchLed(i);
	} while(i<20);
	AllLedON();
	delay(500);
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
	delay(500);

	dataReceived[i]='\0';
	printf("There is %d data and the data is : %s. \n\r",i, dataReceived);
	printf(" Le tab 4 is %x. \n\r The tab5 is : %x. \n\r", dataReceived[4],dataReceived[5]);
	printf(" DECIMAL Le tab 4 is %d. \n\r The tab5 is : %d. \n\r", dataReceived[4],dataReceived[5]);

	//_________________ASCII to Decimal _________________________
	int msbPayload,lsbPayload;
	if (dataReceived[4]>57)
		msbPayload = dataReceived[4]-55;
	else
		msbPayload = dataReceived[4]-48;

	if (dataReceived[5]>57)
		lsbPayload = dataReceived[5]-55;
	else
		lsbPayload = dataReceived[5]-48;



	char PayloadLength=0;
	PayloadLength = 10*msbPayload + lsbPayload;		//take the two numbers as a decimal
	printf("The payload lenght is %d. \n\r",PayloadLength);
*/

/*		//with malloc
   	char * PayloadData = malloc( (PayloadLength+1) * sizeof(char));

   	do{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
		PayloadData[i]= USART_ReceiveData(USART1);//USART1->DR;
		//printf("%x ",PayloadData[i] & 0xFF);	//display the hexa data received
		i++;
	}while(i != (datalength+1));
	printf(" End. ");
*/





/*
	//Test receive (Write what you want)
	int i=0, j=0;

	printf("Data to Send :");
	do{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== RESET);
		i++;
	}while(USART1->DR!=13);  //13==\n
	dataReceived[i]='\0';

	char * dataReceived = malloc(i * sizeof(char));
	printf("Il y a %d octets",i);

	for(j=0 ; j<i ; j++)
	{
		dataReceived[j]= USART1->DR;
	}
	printf("\n\rYour sentence is : %s\n\n\r",dataReceived);
*/

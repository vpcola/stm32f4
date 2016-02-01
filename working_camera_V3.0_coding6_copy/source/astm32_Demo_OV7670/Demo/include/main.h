/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.h
* Author             : MCD Application Team
* Version            : V1.1.1
* Date               : 06/13/2008
* Description        : Header for main.c module
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"
#include "delay.h"
#include "ili9331.h"
#include "ov7670.h"
#include "fifo.h"
#include "hw_config.h"

void Delay (int cnt);


void COBY_BMP(void);
void Key_GPIO_Init(void);
void Demo_Init(void);
void GPIO_Config(void);
//void Delay(u32 nCount);
void SysTick_Config(void);
void DelayXms(unsigned int x) ;
void Decrement_TimingDelay(void);
void SPI1_Init(void);
void LED_GPIO_Init(void);
void SPI1_gpio_Init(void);
unsigned char led_kkk(unsigned char data);
void USART1_init(void);
ErrorStatus Get_HSEStartUpStatus(void);

static vu32 TimingDelay = 0;

//IO定义 **********************************************************************
#define USB_DISCONNECT                    GPIOC  		  //USB所在端口为PROT C
#define USB_DISCONNECT_PIN                GPIO_Pin_15	  //USB软连接与断开控制IO
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOC		  





  



#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

//Error[Li005]: no definition for "Scsi_Sense_Data" [referenced from E:\STM32_FOR_ov7670\stm32_Demo_OV7670 

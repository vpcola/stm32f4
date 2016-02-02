/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "gpio.h"
#include "tim.h"
#include "sd2119.h"
#include "dcmi.h"
#include "ov9655.h"
#include <stdio.h>
#include "shell.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osMutexId lcdMutexHandle;
osSemaphoreId dcmiBinarySemaphoreHandle;
osSemaphoreId buttonBinarySemaphoreHandle;

/* USER CODE BEGIN Variables */
osThreadId shellThreadHandle;
osThreadId usartPrintfTestHandle;
osThreadId gpioToggleLedTestHandle;
osThreadId lcdTestThreadHandle;
osThreadId buttonWatcherHandle;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
extern void MX_FATFS_Init(void);
extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void UsartPrintfTest(void const * argument);
void GpioToggleLedTest(void const * argument);
void LcdTestThread(void const * argument);
void ButtonWatcherThread(void const * argument);

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */
  osMutexDef(lcdMutex);
  lcdMutexHandle = osMutexCreate(osMutex(lcdMutex));
	
  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of dcmiBinarySemaphore */
  osSemaphoreDef(dcmiBinarySemaphore);
  dcmiBinarySemaphoreHandle = osSemaphoreCreate(osSemaphore(dcmiBinarySemaphore), 1);
	
	osSemaphoreDef(buttonBinarySemaphore);
	buttonBinarySemaphoreHandle = osSemaphoreCreate(osSemaphore(buttonBinarySemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  //osThreadDef(usartPrintfTask, UsartPrintfTest, osPriorityNormal, 0, 128);
  //usartPrintfTestHandle = osThreadCreate(osThread(usartPrintfTask), NULL);
  osThreadDef(shellThreadTask, shellStart, osPriorityNormal, 0, 256);
  shellThreadHandle = osThreadCreate(osThread(shellThreadTask), NULL);


  //osThreadDef(gpioToggleTask, GpioToggleLedTest, osPriorityNormal, 0, 128);
  //gpioToggleLedTestHandle = osThreadCreate(osThread(gpioToggleTask), NULL);

  osThreadDef(lcdTestTask, LcdTestThread, osPriorityNormal, 0, 256);
  lcdTestThreadHandle = osThreadCreate(osThread(lcdTestTask), NULL);
	
	osThreadDef(buttonWatcherTask, ButtonWatcherThread, osPriorityNormal, 0, 256);
	buttonWatcherHandle = osThreadCreate(osThread(buttonWatcherTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  LCD_Init();
	LCD_Clear(Black);
	
	PC0_LED_Off();
	
	HAL_TIM_Base_Start(&htim1);
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
void UsartPrintfTest(void const * argument)
{
    int i = 0;
    while(1)
    {
        printf("value of i = [%d]\r\n", i);
        osDelay(100);
			  if (i < 100) 
					i++;
				else
					i = 0;
    }

}

void GpioToggleLedTest(void const * argument)
{
    while(1)
    {
        PC0_LED_Toggle();
        osDelay(1000);
    }
}

void LcdTestThread(void const * argument)
{
    int i = 0;
    char tmpbuff[100];
	  OV9655_IDTypeDef camid;
	
	  // Get the camera id and display it
	  OV9655_ReadID(&camid);
		
		if ( xSemaphoreTake( lcdMutexHandle, ( TickType_t ) 10 ) == pdTRUE )
		{
			sprintf(tmpbuff, "ID1: %X", camid.Manufacturer_ID1);
			LCD_DisplayStringLine(LINE(1), (uint8_t *) tmpbuff);
			sprintf(tmpbuff, "ID2: %X", camid.Manufacturer_ID2);
			LCD_DisplayStringLine(LINE(2), (uint8_t *) tmpbuff);
			sprintf(tmpbuff, "VER: %X", camid.Version);
			LCD_DisplayStringLine(LINE(3), (uint8_t *) tmpbuff);
			xSemaphoreGive( lcdMutexHandle );
		}
	
    while(1)
    {
       sprintf(tmpbuff, "Init %d", i);
			 // Get hold of a shared LCD resource
       if( xSemaphoreTake( lcdMutexHandle, ( TickType_t ) 10 ) == pdTRUE )
       {			
						LCD_DisplayStringLine(LINE(4), (uint8_t *) tmpbuff);
				    xSemaphoreGive( lcdMutexHandle );
			 }
				
       if (i > 99) 
           i = 0;  
       else 
           i++;
			 
			 osDelay(1000);
    }
}

void ButtonWatcherThread(void const * argument)
{
	while(1)
	{
        // this waits for the semaphore to be released
        if (xSemaphoreTake( buttonBinarySemaphoreHandle, portMAX_DELAY ) == pdTRUE )
        {
						if( xSemaphoreTake( lcdMutexHandle, ( TickType_t ) 1000 ) == pdTRUE )
						{					
							PC0_LED_Toggle();
							// Transfer image to lcd
							// Setup camera transfer to LCD
							//OV9655_SetupCamera(QVGA, ToLCD);
					
							//LCD_SetDisplayWindow(0, 0, 320, 240);
							//LCD_WriteRAM_Prepare();

							/* Enable DCMI interface */
							//HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, FSMC_LCD_ADDRESS, (320 * 240 * 2) );			
							
				      xSemaphoreGive( lcdMutexHandle );							
						}
        }		
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == B1_Pin)
	{
     /* Unblock the task by releasing the semaphore. */
			xSemaphoreGiveFromISR( buttonBinarySemaphoreHandle, NULL );
  }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

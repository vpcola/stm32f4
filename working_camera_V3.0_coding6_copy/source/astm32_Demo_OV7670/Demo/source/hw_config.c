
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"
#include "hw_config.h"
#include  "sdcard.h"
#include "Include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void RCC_Config(void);
/* Private functions ---------------------------------------------------------*/


void System_Init(void)
{

  /* RCC configuration */
  RCC_Config();		 //与时钟相关的寄存器配制

  SD_Init();


  
  
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
static void RCC_Config(void)
{

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();	   //将RCC寄存器设备缺省值

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);  //外部高速振荡器HSE 开启

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();	//等待HSE启动

  if (HSEStartUpStatus == SUCCESS)	  //如果HSE晶振稳定且就绪
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  //预取指缓存使能

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);	//设置FLASH存储器延时时钟周期数为2延时周期

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  //设置AHB时钟＝系统时钟

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);	 //设置APB2时钟 ＝ HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);	   //设置APB1时钟 ＝ HCLK/2

    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//设置ADC时钟＝PCLK/6

	//PLL设置
	//PLL输入时钟＝HSE时钟频率   输出 9倍频（HSE×9）
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //72M

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)	 //等待PLL就绪
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//选择PLL作为系统时钟

    /* Wait till PLL is used as system clock source */
	//等待设置系统时钟为PLL成功
	//返回值为	00：HSI  04：HSE  08：PLL
    while (RCC_GetSYSCLKSource() != 0x08)  
    {}
     
     

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO , ENABLE);
      /* DMA1 and DMA2 clock enable */
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
     // 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    
     if(SD_nCD_STATUS() == SD_NOT_IN) //sd insert or not 
       {
         gEquipStatus.SD_status = STA_NODISK;	
         
       }
    else
       {    
	  gEquipStatus.SD_status = disk_initialize(0);
         // SD_insert_IRQ_flag = TRUE;
	 // if(gEquipStatus.SD_status == STA_OK)
	 // {
	 // 	if(SD_WP_STATUS() == SD_YES_WP)	{ gEquipStatus.SD_status = STA_PROTECT; }		
	 // }	  
       }   

  }
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


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
  RCC_Config();		 //��ʱ����صļĴ�������

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
  RCC_DeInit();	   //��RCC�Ĵ����豸ȱʡֵ

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);  //�ⲿ��������HSE ����

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();	//�ȴ�HSE����

  if (HSEStartUpStatus == SUCCESS)	  //���HSE�����ȶ��Ҿ���
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  //Ԥȡָ����ʹ��

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);	//����FLASH�洢����ʱʱ��������Ϊ2��ʱ����

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  //����AHBʱ�ӣ�ϵͳʱ��

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);	 //����APB2ʱ�� �� HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);	   //����APB1ʱ�� �� HCLK/2

    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//����ADCʱ�ӣ�PCLK/6

	//PLL����
	//PLL����ʱ�ӣ�HSEʱ��Ƶ��   ��� 9��Ƶ��HSE��9��
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     //72M

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)	 //�ȴ�PLL����
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//ѡ��PLL��Ϊϵͳʱ��

    /* Wait till PLL is used as system clock source */
	//�ȴ�����ϵͳʱ��ΪPLL�ɹ�
	//����ֵΪ	00��HSI  04��HSE  08��PLL
    while (RCC_GetSYSCLKSource() != 0x08)  
    {}
     
     

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO , ENABLE);
      /* DMA1 and DMA2 clock enable */
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
     // �ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ��
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



#include "stm32f10x_lib.h"



#define FIFO_CS_BIT     GPIO_Pin_9  // pb9
#define FIFO_WRST_BIT   GPIO_Pin_8  // pb8
#define FIFO_RRST_BIT   GPIO_Pin_10 // pb10
#define FIFO_RD_BIT     GPIO_Pin_11  // pb11
#define FIFO_WE_BIT     GPIO_Pin_5  // pb11

#define FIFO_CS_H()    GPIOB->BSRR =FIFO_CS_BIT
#define FIFO_CS_L()    GPIOB->BRR  =FIFO_CS_BIT

#define FIFO_WRST_H()  GPIOB->BSRR =FIFO_WRST_BIT
#define FIFO_WRST_L()  GPIOB->BRR  =FIFO_WRST_BIT

#define FIFO_RRST_H()  GPIOB->BSRR =FIFO_RRST_BIT
#define FIFO_RRST_L()  GPIOB->BRR  =FIFO_RRST_BIT

#define FIFO_RD_H()    GPIOB->BSRR =FIFO_RD_BIT
#define FIFO_RD_L()    GPIOB->BRR  =FIFO_RD_BIT

#define FIFO_WE_H()    GPIOC->BSRR =FIFO_WE_BIT
#define FIFO_WE_L()    GPIOC->BRR  =FIFO_WE_BIT

void  FIFO_Set_GPIO_Config(void);
void FIFO_GPIO_INPUT(void);
#ifndef __TESTRANGE_H__
#define	__TESTRANGE_H__	
#include "typedef.h"

#define RANGE_0			0
#define RANGE_1			1
#define RANGE_2			2
#define RANGE_3			3
#define RANGE_4			4
#define RANGE_5			5
#define RANGE_6			6
#define RANGE_7			7
#define RANGE_8			8
#define RANGE_9			9
#define RANGE_10		10
//#define RANGE_11		11


#define			RANGE_PORT1		GPIOE
#define 		RANGE0				GPIO_Pin_2
#define 		RANGE1				GPIO_Pin_3 
#define 		RANGE2				GPIO_Pin_4
#define 		RANGE3				GPIO_Pin_5

#define			RANGE_PORT2		GPIOB
#define 		RANGE4				GPIO_Pin_5
#define 		RANGE5				GPIO_Pin_6
#define 		RANGE6				GPIO_Pin_7
#define 		RANGE7				GPIO_Pin_8
#define 		RANGE8				GPIO_Pin_9

#define			RANGE_PORT3		GPIOD
#define 		RANGE9				GPIO_Pin_3
#define 		RANGE10				GPIO_Pin_6
#define 		RANGE11				GPIO_Pin_7

#define 	RANGE_ALL_OFF()     do{  \
                            		GPIO_SetBits(RANGE_PORT1, RANGE0|RANGE1|RANGE2|RANGE3);\
                            		GPIO_SetBits(RANGE_PORT2, RANGE4|RANGE5|RANGE6|RANGE7|RANGE8);\
                            		GPIO_SetBits(RANGE_PORT3, RANGE9|RANGE10|RANGE11);\
                        				}while(0)

#define 		RANGE0_ON()		GPIO_ResetBits(RANGE_PORT1, RANGE0)
#define 		RANGE0_OFF()	GPIO_SetBits(RANGE_PORT1, RANGE0)

#define 		RANGE1_ON()		GPIO_ResetBits(RANGE_PORT1, RANGE1)
#define 		RANGE1_OFF()	GPIO_SetBits(RANGE_PORT1, RANGE1)

#define 		RANGE2_ON()		GPIO_ResetBits(RANGE_PORT1, RANGE2)
#define 		RANGE2_OFF()	GPIO_SetBits(RANGE_PORT1, RANGE2)

#define 		RANGE3_ON()		GPIO_ResetBits(RANGE_PORT1, RANGE3)
#define 		RANGE3_OFF()	GPIO_SetBits(RANGE_PORT1, RANGE3)

#define 		RANGE4_ON()		GPIO_ResetBits(RANGE_PORT2, RANGE4)
#define 		RANGE4_OFF()	GPIO_SetBits(RANGE_PORT2, RANGE4)

#define 		RANGE5_ON()		GPIO_ResetBits(RANGE_PORT2, RANGE5)
#define 		RANGE5_OFF()	GPIO_SetBits(RANGE_PORT2, RANGE5)

#define 		RANGE6_ON()		GPIO_ResetBits(RANGE_PORT2, RANGE6)
#define 		RANGE6_OFF()	GPIO_SetBits(RANGE_PORT2, RANGE6)

#define 		RANGE7_ON()		GPIO_ResetBits(RANGE_PORT2, RANGE7)
#define 		RANGE7_OFF()	GPIO_SetBits(RANGE_PORT2, RANGE7)

#define 		RANGE8_ON()		GPIO_ResetBits(RANGE_PORT2, RANGE8)
#define 		RANGE8_OFF()	GPIO_SetBits(RANGE_PORT2, RANGE8)

#define 		RANGE9_ON()		GPIO_ResetBits(RANGE_PORT3, RANGE9)
#define 		RANGE9_OFF()	GPIO_SetBits(RANGE_PORT3, RANGE9)

#define 		RANGE10_ON()	GPIO_ResetBits(RANGE_PORT3, RANGE10)
#define 		RANGE10_OFF()	GPIO_SetBits(RANGE_PORT3, RANGE10)

//#define 		RANGE11_ON()	GPIO_ResetBits(RANGE_PORT3, RANGE11)
//#define 		RANGE11_OFF()	GPIO_SetBits(RANGE_PORT3, RANGE11)

#define DELAY_OFF()     GPIO_ResetBits(GPIOE, GPIO_Pin_6)

#define DELAY_ON()    	GPIO_SetBits(GPIOE, GPIO_Pin_6)

void RangeGPIO_Configuration(void);
void xSetRange(u8 nRng);
u8 xSelectRange(vu16 voltaRange, vu8 *mv_OnePixel_Y);

#endif

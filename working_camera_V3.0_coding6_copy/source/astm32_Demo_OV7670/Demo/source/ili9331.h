#include "stm32f10x_lib.h"


///////////////////////////////////////////////////////
#define	BLACK	0x0000
#define	BLUE	0x001F
#define	RED 	0xF800
#define	GREEN 	0x07E0
#define CYAN	0x07FF
#define MAGENTA 0xF81F
#define YELLOW	0xFFE0
#define WHITE	0xFFFF		


#define ID_AM         001


#define DATA_LCD_PORT GPIOB->ODR
#define DATA_LCD_PIN  GPIOB->IDR

//#define DATA_INPUT()   DATA_LCD_DDR = 0x00;
//#define DATA_OUPUT()   DATA_LCD_DDR = 0xFF;

#define LCD_CS_BIT   GPIO_Pin_12  // pb12
#define LCD_RS_BIT   GPIO_Pin_13  // pb13
#define LCD_RD_BIT   GPIO_Pin_15  //pb15
#define LCD_WR_BIT   GPIO_Pin_14  //pb14
#define LCD_BL_BIT   GPIO_Pin_15  // pc15
#define LCD_RST_BIT   GPIO_Pin_15  // pa15


#define LCD_CS_H() GPIOB->BSRR =LCD_CS_BIT
#define LCD_CS_L() GPIOB->BRR =LCD_CS_BIT

#define LCD_RS_H() GPIOB->BSRR =LCD_RS_BIT
#define LCD_RS_L() GPIOB->BRR =LCD_RS_BIT

#define LCD_WR_H() GPIOB->BSRR =LCD_WR_BIT
#define LCD_WR_L() GPIOB->BRR =LCD_WR_BIT

#define LCD_RD_H() GPIOB->BSRR =LCD_RD_BIT
#define LCD_RD_L() GPIOB->BRR =LCD_RD_BIT

#define LCD_BL_H()  GPIOC->BSRR =LCD_BL_BIT
#define LCD_BL_L()  GPIOC->BRR =LCD_BL_BIT

#define LCD_RST_H()  GPIOA->BSRR =LCD_RST_BIT
#define LCD_RST_L()  GPIOA->BRR =LCD_RST_BIT

void  LCD_RST_GPIO_Config(void);
void  LCD_GPIO_OUTPUT(void);
void  LCD_GPIO_INPUT(void);
void  LCD_Set_GPIO_Config(void);
void  LCD_BL_GPIO_Config(void);
unsigned int LCD_RD_DATA16(void);//¶Á»ý´æÆ÷Êý¾Ý  

void LCD_WR_REG(unsigned int index,unsigned int data);
void LCD_WR_REG16(unsigned int index);//Ð´»ý´æÆ÷ÃüÁî
void LCD_WR_DATA16(unsigned int data);//Î÷»ý´æÆ÷Êý¾Ý
void LCD_Init(void);
void LCD_Clear(unsigned int Color);
void LCD_write_english(unsigned char data,unsigned int color,unsigned int xcolor);//Ð´×Ö·û
void LCD_write_english_string(unsigned int x,unsigned int y,unsigned char *s,unsigned int color,unsigned int xcolor);//Ó¢ÎÄ×Ö·û´®ÏÔÊ¾
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos);
void Lcd_SetBox(unsigned int xStart,unsigned int yStart,unsigned int xLong,unsigned int yLong);
void LCD_write_SUM(unsigned int x,unsigned int y,unsigned char SUM,unsigned int color,unsigned int xcolor);//Ð´Êý×Ö
void Test(void);
void Test_tu(void);
void BMP_write_english(unsigned char x,unsigned char val_d,unsigned char color);//Ð´×Ö·û
void LCD_DRAW_PIXEL(unsigned int x, unsigned int y);
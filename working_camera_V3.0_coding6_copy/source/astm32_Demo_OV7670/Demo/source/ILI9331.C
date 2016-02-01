#include  "english_16x8.h"
#include"ILI9331.h"
#include"main.h"
extern unsigned char val_Speed[16][100];
/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configures the different GPIO ports pins.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void Delay (int cnt) {

  cnt <<= 18;
  while (cnt--);
}


void LCD_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void LCD_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void  LCD_Set_GPIO_Config(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =LCD_CS_BIT|LCD_RS_BIT|LCD_WR_BIT|LCD_RD_BIT;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  LCD_CS_H(); 
  LCD_RD_H();
  LCD_RS_H();   
  LCD_WR_H();
}
void  LCD_BL_GPIO_Config(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin =LCD_BL_BIT;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void  LCD_RST_GPIO_Config(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin =LCD_BL_BIT;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

void LCD_WR_DATA16(unsigned int data)//д
{
     
     LCD_RS_H();
     LCD_CS_L();
     
     GPIOB->ODR=(GPIOB->ODR&0XFF00)| (unsigned char)(data>>8);    
     LCD_WR_L();
     LCD_WR_H();
     
     GPIOB->ODR=(GPIOB->ODR&0XFF00)|(unsigned char)data;    
     LCD_WR_L();
     LCD_WR_H();
     LCD_CS_H();
  
}
void LCD_WR_REG16(unsigned int index)//д
{
    
     LCD_RS_L();
     LCD_CS_L();  
     
     GPIOB->ODR=(GPIOB->ODR&0XFF00)| (unsigned char)(index>>8);     
     LCD_WR_L();
     LCD_WR_H();
     
     GPIOB->ODR=(GPIOB->ODR&0XFF00)|(unsigned char)index;    
     LCD_WR_L();
     LCD_WR_H();
     LCD_CS_H();
}
unsigned int LCD_RD_DATA16(void)//
{
     unsigned int data;
     LCD_WR_H();
     LCD_RS_H();
     LCD_GPIO_INPUT();
     LCD_CS_L(); 
 
     LCD_RD_L();
     
     LCD_RD_H();
     data=(0x00ff&DATA_LCD_PIN); 
     data=data<<8;
     
	 
     LCD_RD_L();
     
     LCD_RD_H();
     data|=(0x00ff&DATA_LCD_PIN);
     
     LCD_CS_H();
     LCD_GPIO_OUTPUT();
     return data;
  
}
void LCD_WR_REG(unsigned int index,unsigned int data)
{
     LCD_WR_REG16(index);
     LCD_WR_DATA16(data);
}


void LCD_Init(void)
{   
      LCD_RST_GPIO_Config();
      LCD_BL_GPIO_Config();
      LCD_GPIO_OUTPUT();
      LCD_Set_GPIO_Config();   
      LCD_RST_L();
    
     
      LCD_BL_L(); 
       Delay(30);
       LCD_RST_H();
  //  delay_ms(5000);
      
      LCD_WR_REG(0x00a4,0x0001); 
      LCD_WR_REG(0x0000,0x0000); 
      Delay(15);
      LCD_WR_REG(0x0000,0x0000); 
      LCD_WR_REG(0x0001,0x0100);//S 
      LCD_WR_REG(0x0002,0x0000); // set 1 line inversion
      Delay(5);
        // LCD_WR_REG(0x03, 0x10B0); // set GRAM write direction and BGR=1.
#if   ID_AM==000       
      LCD_WR_REG(0x0003,0x1000);//Ļת TFM=0,TRI=0,SWAP=1,16 bits system interface  swap RGB to BRG˴ORGHWM Ϊ0
#elif ID_AM==001        
      LCD_WR_REG(0x0003,0x1008);      
#elif ID_AM==010  
     LCD_WR_REG(0x0003,0x1010);        
#elif ID_AM==011
     LCD_WR_REG(0x0003,0x1018);
#elif ID_AM==100  
     LCD_WR_REG(0x0003,0x1020);      
#elif ID_AM==101  
     LCD_WR_REG(0x0003,0x1028);      
#elif ID_AM==110  
     LCD_WR_REG(0x0003,0x1030);      
#elif ID_AM==111  
     LCD_WR_REG(0x0003,0x1038);
#endif    
         LCD_WR_REG(0x04, 0x0000); // Resize register
         LCD_WR_REG(0x08, 0x0303); // set the back porch and front porch
         LCD_WR_REG(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
         LCD_WR_REG(0x0A, 0x0008); // FMARK function
         LCD_WR_REG(0x0C, 0x0001); // RGB interface setting
         LCD_WR_REG(0x0D, 0x0000); // Frame marker Position
         LCD_WR_REG(0x0F, 0x0000); // RGB interface polarity
//Power On sequence //
         LCD_WR_REG(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
         LCD_WR_REG(0x11, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
         LCD_WR_REG(0x12, 0x0000); // VREG1OUT voltage
         LCD_WR_REG(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
//	 LCD_WR_REG(0x07, 0x0001); // 
         
         Delay(5); // Delay 5ms 
         LCD_WR_REG(0x07, 0x0101); 
         Delay(5); // Delay 5ms 

         
         LCD_WR_REG(0x10, 0x0620); // SAP, BT[3:0], AP, DSTB, SLP, STB
         LCD_WR_REG(0x11, 0x0237); // DC1[2:0], DC0[2:0], VC[2:0]
         LCD_WR_REG(0x0e, 0x0020); 
         Delay(30); // Delay 50ms
        
         LCD_WR_REG(0x12, 0x013a); // Internal reference voltage= Vci;
        Delay(30); // Delay 50ms
         LCD_WR_REG(0x13, 0x0700); // Set VDV[4:0] for VCOM amplitude
         LCD_WR_REG(0x29, 0x0048); // Set VCM[5:0] for VCOMH
         LCD_WR_REG(0x2A, 0x003d);
      //   LCD_WR_REG(0x2B, 0x000f); // Set Frame Rate
         Delay(30); // Delay 50ms        

#if   ID_AM==000         
	     LCD_WR_REG(0x0020,0x00ef);//GRAMˮƽʼλ
	     LCD_WR_REG(0x0021,0x013f);      
#elif ID_AM==001
	     LCD_WR_REG(0x0020,0x00ef);
	     LCD_WR_REG(0x0021,0x013f);      
#elif ID_AM==010
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x013f);      
#elif ID_AM==011
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x013f);       
#elif ID_AM==100
	     LCD_WR_REG(0x0020,0x00ef);
	     LCD_WR_REG(0x0021,0x0000);      
#elif ID_AM==101  
	     LCD_WR_REG(0x0020,0x00ef);
	     LCD_WR_REG(0x0021,0x0000);      
#elif ID_AM==110
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x0000);      
#elif ID_AM==111
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x0000);         
#endif  
// ----------- Adjust the Gamma Curve ILI9328----------//
	 /*
		LCD_WR_REG(0x0030,0x0007);
		LCD_WR_REG(0x0031,0x0707);
		LCD_WR_REG(0x0032,0x0107);
		LCD_WR_REG(0x0035,0x0206);
		LCD_WR_REG(0x0036,0x0406);
		LCD_WR_REG(0x0037,0x0101);
		LCD_WR_REG(0x0038,0x0101);
		LCD_WR_REG(0x0039,0x0207);
		LCD_WR_REG(0x003C,0x0504);
		LCD_WR_REG(0x003D,0x0806);*/ 	
/************GAMMA CONTROL***********/
		LCD_WR_REG(0X0030,0X0902);//R30~39h:  Control
		LCD_WR_REG(0X0031,0X5615);
		LCD_WR_REG(0X0032,0X0603);
		LCD_WR_REG(0X0033,0X1801);
		LCD_WR_REG(0X0034,0X0000);
		LCD_WR_REG(0X0035,0X0118);
		LCD_WR_REG(0X0036,0X5306);
		LCD_WR_REG(0X0037,0X1506);
		LCD_WR_REG(0X0038,0X0209);
		LCD_WR_REG(0X0039,0X3333);
            
		 
//------------------ Set GRAM area ---------------//
         LCD_WR_REG(0x0050, 0x0000); // Horizontal GRAM Start Address
         LCD_WR_REG(0x0051, 0x00EF); // Horizontal GRAM End Address
         LCD_WR_REG(0x0052, 0x0000); // Vertical GRAM Start Address
         LCD_WR_REG(0x0053, 0x013F); // Vertical GRAM Start Address
         LCD_WR_REG(0x0060, 0xa700); // Gate Scan Line
         LCD_WR_REG(0x0061, 0x0001); // NDL,VLE, REV
         LCD_WR_REG(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//

//-------------- Panel Control -------------------//
         LCD_WR_REG(0x90, 0X0011);
         LCD_WR_REG(0x92, 0x0000);
         LCD_WR_REG(0x93, 0x0003);
         LCD_WR_REG(0x95, 0x0110);
         LCD_WR_REG(0x97, 0x0000);
         LCD_WR_REG(0x98, 0x0000);

	 Delay(5); // Delay 5ms	 
         LCD_WR_REG(0x07, 0x0173); 
         Delay(5); // Delay 5ms
	 LCD_Clear(000);
        
	 Delay(2); // Delay 5ms
         LCD_BL_H();
}


/**********************************************
Lcdȫ
ܣLcdΪָɫ
ڲcolor ָLcdȫɫ RGB(5-6-5)
ֵ
***********************************************/
void LCD_Clear(unsigned int Color)
{
  unsigned long index = 0;
  
#if   ID_AM==000         
	     LCD_WR_REG(0x0020,0x00ef);//GRAMˮƽʼλ
	     LCD_WR_REG(0x0021,0x013f);      
#elif ID_AM==001
	     LCD_WR_REG(0x0020,0x00ef);
	     LCD_WR_REG(0x0021,0x013f);      
#elif ID_AM==010
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x013f);      
#elif ID_AM==011
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x013f);       
#elif ID_AM==100
	     LCD_WR_REG(0x0020,0x00ef);
	     LCD_WR_REG(0x0021,0x0000);      
#elif ID_AM==101  
	     LCD_WR_REG(0x0020,0x00ef);
	     LCD_WR_REG(0x0021,0x0000);      
#elif ID_AM==110
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x0000);      
#elif ID_AM==111
	     LCD_WR_REG(0x0020,0x0000);
	     LCD_WR_REG(0x0021,0x0000);         
#endif     

  LCD_WR_REG(0x0050,0x00);//ˮƽ GRAMʼλ
  LCD_WR_REG(0x0051,239);//ˮƽGRAMֹλ
  LCD_WR_REG(0x0052,0x00);//ֱGRAMʼλ
  LCD_WR_REG(0x0053,319);//ֱGRAMֹλ 
  LCD_WR_REG16(0x0022);
  
 // LCD_RD_H();
//  LCD_RS_H();
  //LCD_CS_L();   
  for(index = 0; index < 76800; index++)
  {
   // LCD_WR_DATA16(Color);
 
    LCD_WR_DATA16(Color);
 
   
  } 
  // LCD_CS_H();
}

void LCD_write_english(unsigned char data,unsigned int color,unsigned int xcolor)//дַ
{

  unsigned char avl,i,n;
   LCD_WR_REG16(0x0022);  
  for (i=0;i<16;i++)
  { 
      avl=english[data-32][i];
	  for (n=0;n<8;n++)
	   {
	     if(avl&0x80) LCD_WR_DATA16(color);
             else LCD_WR_DATA16(xcolor);
           
	     avl<<=1;
	   }
	}
}
void LCD_write_english_string(unsigned int x,unsigned int y,unsigned char *s,unsigned int color,unsigned int xcolor)//Ӣַʾ
{
 unsigned int k=0;
 while (*s) 
  {
     Lcd_SetBox(y,x+k,15,8);
     LCD_write_english( *s,color,xcolor);
     k=k+8;
     s++;
  
  }

}  

void LCD_write_SUM(unsigned int x,unsigned int y,unsigned char SUM,unsigned int color,unsigned int xcolor)//д
{

  unsigned char avl,i,n;
   Lcd_SetBox(y,x,15,8);
   LCD_WR_REG16(0x0022);  
  for (i=0;i<16;i++)
  { 
      avl=english[SUM+16][15-i];
	  for (n=0;n<8;n++)
	   {
	     if(avl&0x80) LCD_WR_DATA16(color);
             else LCD_WR_DATA16(xcolor);
           
	     avl<<=1;
	   }
	}
}
/**********************************************
Lcdѡ
ܣѡLcdָľ

ע⣺xStart yStartĻתı䣬λǾοĸ

ڲxStart xʼ
          ySrart yֹ
          xLong Ҫѡεx򳤶
          yLong  Ҫѡεy򳤶
ֵ
***********************************************/
void Lcd_SetBox(unsigned int xStart,unsigned int yStart,unsigned int xLong,unsigned int yLong)
{
  
#if ID_AM==000    
	LCD_SetCursor(xStart+xLong-1,312-yStart+yLong-1);

#elif ID_AM==001
	LCD_SetCursor(xStart+xLong-1,312-yStart+yLong-1);
     
#elif ID_AM==010
	LCD_SetCursor(xStart,312-yStart+yLong-1);
     
#elif ID_AM==011 
	LCD_SetCursor(xStart,312-yStart+yLong-1);
     
#elif ID_AM==100
	LCD_SetCursor(xStart+xLong-1,312-yStart);     
     
#elif ID_AM==101
	LCD_SetCursor(xStart+xLong-1,312-yStart);     
     
#elif ID_AM==110
	LCD_SetCursor(xStart,312-yStart); 
     
#elif ID_AM==111
	LCD_SetCursor(xStart,312-yStart);  
     
#endif
     
	LCD_WR_REG(0x0050,xStart);//ˮƽ GRAMʼλ
	LCD_WR_REG(0x0051,xStart+xLong-1);//ˮƽGRAMֹλ
	LCD_WR_REG(0x0052,312-yStart);//ֱGRAMʼλ
	LCD_WR_REG(0x0053,312-yStart+yLong-1);//ֱGRAMֹλ 
}


/*************************************************
Lcd㶨λ
ܣָ320240ҺϵһΪдݵʼ
ڲx  0~239
          y  0~319
ֵ
*************************************************/
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos)
{
 // CLR_RS;
  LCD_WR_REG(0x20, Xpos);
  LCD_WR_REG(0x21, Ypos);
}







void Test(void)
{       
        unsigned long n,i;
       LCD_WR_REG(0x0020,0x0000);//GRAMˮƽʼλ
       LCD_WR_REG(0x0021,0x0000); 
        for(i=0;i<7;i++)
          
        {       
        
            
	        LCD_WR_REG(0x0050,0x00);//ˮƽ GRAMʼλ
	        LCD_WR_REG(0x0051,239);//ˮƽGRAMֹλ
	        LCD_WR_REG(0x0052,0x00);//ֱGRAMʼλ
	        LCD_WR_REG(0x0053,319);//ֱGRAMֹλ 
           
            LCD_WR_REG16(0x0022);       
                for(n=0;n<76800;n++)
                { 
                  
                  
                  if(i==0)LCD_WR_DATA16(BLUE);   
                  if(i==1)LCD_WR_DATA16(RED); 
                  if(i==2)LCD_WR_DATA16(GREEN);
                  if(i==3)LCD_WR_DATA16(CYAN); 
                  if(i==4)LCD_WR_DATA16(MAGENTA); 
                  if(i==5)LCD_WR_DATA16(YELLOW);
				  
                  if(i==6)LCD_WR_DATA16(0x0102); 
                }    
                                    
        }       
}

void BMP_write_english(unsigned char x,unsigned char val_d,unsigned char color)//дַ
{

  unsigned char avl,i,n;
  for (i=0;i<16;i++)
  { 
      avl=english[val_d-32][i];
	  for (n=0;n<8;n++)
	   {
	     if(avl&0x1) val_Speed[15-i][(7-n)+x*8]=color;
             else val_Speed[15-i][(7-n)+x*8]=0;
           
	     avl>>=1;
	   }
	}
}

void LCD_DRAW_PIXEL(unsigned int x, unsigned int y)
{

       LCD_WR_REG(0x0020,0x0000);//GRAMË®Æ½ÆðÊ¼Î»ÖÃ
       LCD_WR_REG(0x0021,0x0000); 
       
            
	LCD_WR_REG(0x0050,y);//Ë®Æ½ GRAMÆðÊ¼Î»ÖÃ
	LCD_WR_REG(0x0051,y+1);//Ë®Æ½GRAMÖÕÖ¹Î»ÖÃ
	LCD_WR_REG(0x0052,x);//´¹Ö±GRAMÆðÊ¼Î»ÖÃ
	LCD_WR_REG(0x0053,x+1);//´¹Ö±GRAMÖÕÖ¹Î»ÖÃ 
           
        LCD_WR_REG16(0x0022);       
        LCD_WR_DATA16(0xF800);
}
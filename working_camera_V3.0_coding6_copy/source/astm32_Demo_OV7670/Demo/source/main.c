
#include "main.h"
#include "Variable.h"
#include "Include.h"

 

void COBY_BMP(void);
const unsigned char bmp_tou[54]=
	{
	  0x42,0x4d,0x36,0x58, 0x02,0x00,0x00,0x00, 0x00,0x00,0x36,0x00, 0x00,0x00,0x28,0x00,//16
	  0x00,0x00,0xa0,0x00, 0x00,0x00,0x78,0x00, 0x00,0x00,0x01,0x00, 0x10,0x00,0x00,0x00,//32
	  0x00,0x00,0x00,0x58, 0x02,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,//48
	  0x00,0x00,0x00,0x00, 0x00,0x00,													 //54
	} ;

static volatile ErrorStatus HSEStartUpStatus = SUCCESS;
volatile unsigned int  b=1,c=0;
volatile unsigned char val_Speed[16][100];

volatile unsigned char usart_data,usart_rx=0,usart_rx1=0;



     FATFS fs;            // Work area (file system object) for logical drive  
     FIL fsrca;      // file objects      
  //   unsigned char buffer[256],a=0xff;   // file copy buffer  
     FRESULT res;         // FatFs function common result code      
     UINT br, bw;         // File R/W count   
     unsigned char buffera[2];
     XCHAR file_name[]="IMG0000.bmp";
     unsigned long a;
     unsigned int x,y,val;
   //  unsigned char TxBuffer1[16]; 
     volatile unsigned char RxBuffer1[16];

   //  unsigned int r,g,b;

unsigned int color_in_16bit(unsigned char col1, unsigned char col2);    
void color_to_rgb(unsigned char *R, unsigned char *G, unsigned char *B, unsigned char col1, unsigned char col2);

unsigned char Red;
unsigned char Green;
unsigned char Blue;

void filter1(unsigned char my_array[][120][2]);
void filter2(unsigned char my_array[][120][2]);
void filter3(unsigned char my_array[][120][2]);
void filter4(unsigned char my_array[][120][2]);
void filter6(int *area, int *perimeter, unsigned char my_array[][120][2]);
void filter5(int *cent_x, int *cent_y, unsigned char my_array[][120][2]);

int xx, yy;

unsigned char string[10];
void itoc(int _i);    
void itoc(int _i){
  int c=0;
  int __i=_i;
  int val;
  char tmp[20];
  int b;
  int k;

  for(;__i>0;++c)
    {
      __i=__i/10;
      __i=__i*10;
      val=_i-__i;
      _i=_i/10;
      __i=__i/10;
      tmp[c]=val+'0';
    }

  b=c;

  for(k=-1;k<b+1;++k){
    string[k]=tmp[c];
    --c;}

  string[b]='\0';
}
     
void neuronai(float *w1, float *w2);
int get_calc_val(float w1, float w2, float x1, float x2);

void to_file(unsigned char my_array[][120][2]);
void to_file(unsigned char my_array[][120][2])
{

  f_mount(0, &fs);  
  res = f_open(&fsrca,file_name, FA_CREATE_ALWAYS | FA_WRITE);  
  res = f_write(&fsrca, bmp_tou,54, &bw); 
 
  
  for(int q = 0; q < 120; q++){
          for(int w = 0; w < 160; w++){
           buffera[0]=my_array[w][q][0];
           buffera[1]=my_array[w][q][1];
          res = f_write(&fsrca, buffera,2, &bw);   
          } 
}

 f_close(&fsrca);   
 f_mount(0, NULL);
 
 file_name[6]++;
        if(file_name[6]>'9')
        {
          file_name[6]='0';
          file_name[5]++;
          if(file_name[5]>'9')
          {
            file_name[5]='0';
            file_name[4]++;
            if(file_name[4]>'9')
              {
                 file_name[4]='0';
                 file_name[3]++;            
              } 
          }
          

        }

}

int main(void)
{ 
  
//static unsigned char my_string[10];
int area_;
int perimeter_;
int roundness;

float w1, w2;
unsigned char temp=0;


 static unsigned char image_array[160][120][2];
 
  /* Initialize the Demo */
  System_Init();
  FIFO_Set_GPIO_Config();
 
  
  LCD_Init();
  
 
  
  
  LCD_write_english_string(5,200-32,"x: ",0Xf800,0X0000);
  LCD_write_english_string(5,200-48,"y: ",0Xf800,0X0000); 
  
  LCD_write_english_string(5,200,"start",0Xf800,0X0000);
  
  OV7670_EXTI_Config();
  while(1!=OV7670_init());
  
  //rdOV7670Reg(0x0a, &temp);
  //itoc(temp);
  //LCD_write_english_string(5,200-16,string,0Xf800,0X0000);
  
  LCD_write_english_string(5,200+16,"OV7670 Init....ok",0Xf800,0X0000);
  delay_ms(1000);
  ov7670_Interrupts_Config();

  
  
  
  //neuronai(&w1, &w2);
  
  
  
 
  //LCD_write_english_string(5,200-16,"end",0Xf800,0X0000);
  
  while (1)
  {    
 
      if(b)
      {   
          
         b=0; 
        }
      else 
      {
        LCD_GPIO_INPUT(); 
        
        
   
        FIFO_CS_L(); 
          b=1;
          FIFO_RRST_L(); 
          GPIOB->BRR  =FIFO_RD_BIT;  
          GPIOB->BSRR =FIFO_RD_BIT; 
          FIFO_RRST_H();     
          GPIOB->BRR  =FIFO_RD_BIT;  
          GPIOB->BSRR =FIFO_RD_BIT; 
          
   	for(x=0;x<120;x++)
	{
	   
	     for(y=0;y<160;y++)
	      {

                GPIOB->BRR  =FIFO_RD_BIT;
                GPIOB->BSRR =FIFO_RD_BIT; //1
                
                val=((unsigned char)GPIOB->IDR); 
                buffera[1]=((unsigned char)val)>>1; 
                val=(val<<8);
                GPIOB->BRR  =FIFO_RD_BIT;
                GPIOB->BSRR =FIFO_RD_BIT;
                
                val|=(unsigned char)GPIOB->IDR; 
                
           
                buffera[0]=(unsigned char)(val&0x1f)|((val>>1)&0xe0); 
                image_array[y][x][0]=buffera[0];
                image_array[y][x][1]=buffera[1];
                
                
                
	           
	     
                  GPIOB->BRR  =FIFO_RD_BIT;
		  GPIOB->BSRR =FIFO_RD_BIT; 
		  GPIOB->BRR  =FIFO_RD_BIT;         
		  GPIOB->BSRR =FIFO_RD_BIT;
                   
	    }
		for(int hah = 0; hah < 320; hah++){
                  GPIOB->BRR  =FIFO_RD_BIT;
		  GPIOB->BSRR =FIFO_RD_BIT; 
		  GPIOB->BRR  =FIFO_RD_BIT;         
		  GPIOB->BSRR =FIFO_RD_BIT;
                  }
        
	 
	
	}  
      
            delay_ms(200);
       b=0;
        
       

        LCD_GPIO_OUTPUT();
        LCD_WR_REG(0x0003,0x1008);
          
          

    
 
 
 
//to_file(image_array);
filter1(image_array);//to_file(image_array);
filter2(image_array);//to_file(image_array);
filter3(image_array);//to_file(image_array);
filter4(image_array);//to_file(image_array);
filter6(&area_, &perimeter_, image_array);//to_file(image_array);
filter5(&xx, &yy, image_array);//to_file(image_array);
  
   
  
    
          
          
        LCD_WR_REG(0x0020,0x0000);
        LCD_WR_REG(0x0021,0x0000); 
     
        LCD_WR_REG(0x0050,0x00);
	LCD_WR_REG(0x0051,119);
	LCD_WR_REG(0x0052,0x00);
	LCD_WR_REG(0x0053,159); 
           
        LCD_WR_REG16(0x0022);       
        for(x=119;x!=0;x--){
        for(y=0;y<160;y++){
	LCD_WR_DATA16(color_in_16bit(image_array[y][x][1], image_array[y][x][0]));   
        }}
        
        
        
        LCD_write_english_string(26,200-32,"   ",0X07E0,0X0000);
        LCD_write_english_string(26,200-48,"   ",0X07E0,0X0000);
        itoc(xx);
        LCD_write_english_string(26,200-32,string,0X07E0,0X0000);
        itoc(yy);
        LCD_write_english_string(26,200-48,string,0X07E0,0X0000); 
        
        perimeter_ = perimeter_ * perimeter_;
        roundness = 4 * 50 * area_;
        roundness = roundness / (float)perimeter_;
        
        
        
        LCD_write_english_string(26,200-80,"   ",0X07E0,0X0000);
        itoc(roundness);
        LCD_write_english_string(26,200-80,string,0X07E0,0X0000); 
        
        if(roundness > 33){
        LCD_write_english_string(26,200-96,"circle",0X07E0,0X0000);
        }
        else{
        LCD_write_english_string(26,200-96,"cubic ",0X07E0,0X0000); 
        }
        
        
        
        
        
        b=1;  
       
        
        
      }
      
      
      
      
      }   
         
       
 }


int get_calc_val(float w1, float w2, float x1, float x2){
    
    float a = x1*w1 + x2*w2;
    if (a>0.5)return 1; 
    else return 0;
}


void neuronai(float *w1, float *w2){
    
    
float x2[8] = {5, 10, 20, 34, 36, 38, 52, 62};
float x1[8] = {5, 10, 20, 34, 36, 38, 52, 62};


*w1= 0.1;
*w2= 0.6;

float b = 0;
int T[8]={1,1,1,1,-1,-1,-1,-1};
int flag = 1;
 
float y[8]={0,0,0,0,0,0,0,0};
float Y[8]={0,0,0,0,0,0,0,0};
float e = 0;

 while(flag == 1){
    for(int m = 0; m < 8; m++){ 
        y[m]=x1[m]*(*w1)+x2[m]*(*w2) + b;       
        if (y[m]>0.5){Y[m]=1;}
        else{Y[m]=-1;}
        }    
       
    e=0;
    
    for(int m = 0; m < 8; m++){
        e=e+T[m]-Y[m];}
    
    e=e/8;
    if (e==0){flag = 0;}
    else{         
         for(int m = 0; m < 8; m++){
            *w1=*w1+0.0001*e*x1[m];
            *w2=*w2+0.0001*e*x2[m]; 
            b = b + 0.0001 * e ;
            }
    
}
}
 
    
    }


void filter1(unsigned char my_array[][120][2]){
  for(x = 0; x < 120; x++){
          for(y = 0; y < 160; y++){
          color_to_rgb(&Red, &Green, &Blue, my_array[y][x][1], my_array[y][x][0]);

          if(Red > Blue + Green + 3){
           my_array[y][x][1]=0x7F;    
           my_array[y][x][0]=0xFF;} 
          }} 
}


void filter2(unsigned char my_array[][120][2]){
          for(int k = 0; k < 120; k++){
          for(int j = 0; j < 160; j++){

          int tmp = 0;
    
         if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF) && (j > 0) && (j < 158) && (k > 0) && (k < 118))
        {
    
        for(int m = -1; m < 2; m++){
	for(int n = -1; n < 2; n++){
          
            if((my_array[j+m][k+n][1] == 0x7F) && (my_array[j+m][k+n][0] == 0xFF)){tmp++;}
            }}    
            if(tmp < 5){
             my_array [j][k][0] = 0x25;
             my_array [j][k][1] = 0x25;
             tmp=0;
             }       
            }
    }
}
}


void filter3(unsigned char my_array[][120][2]){
  for(int k = 0; k < 120; k++){
  for(int j = 0; j < 4  ; j++){    
  if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF))
    my_array [j][k][0] = 0x25;
    my_array [j][k][1] = 0x25;
  }}    


  for(int k = 0;   k < 120; k++){
  for(int j = 157; j < 160; j++){    
  if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF))
    my_array [j][k][0] = 0x25;
    my_array [j][k][1] = 0x25;
  }}

  for(int k = 0; k < 4;   k++){
  for(int j = 0; j < 160; j++){    
  if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF))
    my_array [j][k][0] = 0x25;
    my_array [j][k][1] = 0x25;
  }}    


  for(int k = 117; k < 120; k++){
  for(int j = 0;   j < 160; j++){    
  if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF))
    my_array [j][k][0] = 0x25;
    my_array [j][k][1] = 0x25;
  }}
}      



void filter4(unsigned char my_array[][120][2]){
    for(int k = 0; k < 110; k++){
    for(int j = 0; j < 150; j++){
    
          if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF)){
            
           if(my_array[j+9][k][0] == 0xFF){
             my_array[j+8][k][0]=0xFF;
             my_array[j+8][k][1]=0x7F;
             } 
            
            if(my_array[j+8][k][0] == 0xFF){
             my_array[j+7][k][0]=0xFF;
             my_array[j+7][k][1]=0x7F;
             }
             
             if(my_array[j+7][k][0] == 0xFF){
             my_array[j+6][k][0]=0xFF;
             my_array[j+6][k][1]=0x7F;
             }
               if(my_array[j+6][k][0] == 0xFF){
             my_array[j+5][k][0]=0xFF;
             my_array[j+5][k][1]=0x7F;
             }
              if(my_array[j+5][k][0] == 0xFF){
             my_array[j+4][k][0]=0xFF;
             my_array[j+4][k][1]=0x7F;
             }
           
           if(my_array[j+4][k][0] == 0xFF){
             my_array[j+3][k][0]=0xFF;
             my_array[j+3][k][1]=0x7F;
             }
          
             if(my_array[j+3][k][0] == 0xFF){
             my_array[j+2][k][0]=0xFF;
             my_array[j+2][k][1]=0x7F;
             }
           
           if(my_array[j+2][k][0] == 0xFF){
             my_array[j+1][k][0]=0xFF;
             my_array[j+1][k][1]=0x7F;
             }
           
           
            if(my_array[j][k+9][0] == 0xFF){
             my_array[j][k+8][0]=0xFF;
             my_array[j][k+8][1]=0x7F;
             }
            if(my_array[j][k+8][0] == 0xFF){
             my_array[j][k+7][0]=0xFF;
             my_array[j][k+7][1]=0x7F;
             }
            if(my_array[j][k+7][0] == 0xFF){
             my_array[j][k+6][0]=0xFF;
             my_array[j][k+6][1]=0x7F;
             }
           if(my_array[j][k+6][0] == 0xFF){
             my_array[j][k+5][0]=0xFF;
             my_array[j][k+5][1]=0x7F;
             }
             if(my_array[j][k+5][0] == 0xFF){
             my_array[j][k+4][0]=0xFF;
             my_array[j][k+4][1]=0x7F;
             }
            if(my_array[j][k+4][0] == 0xFF){
             my_array[j][k+3][0]=0xFF;
             my_array[j][k+3][1]=0x7F;
             }
               if(my_array[j][k+3][0] == 0xFF){
             my_array[j][k+2][0]=0xFF;
             my_array[j][k+2][1]=0x7F;
             }
             if(my_array[j][k+2][0] == 0xFF){
             my_array[j][k+1][0]=0xFF;
             my_array[j][k+1][1]=0x7F;
             }
            
             
             
           
           
             
             
             
           
           
             
            
             
             
             
             }
 
    }}
  
}











void filter5(int *cent_x, int *cent_y, unsigned char my_array[][120][2]){

    
    int i1 = 0;
    int i2 = 0;

    int num1=0;

    int xmin=0;
    int xmax=0;
    
    int ymin=0;
    int ymax=0;

    int center1_x;
    
    int center1_y;     
 
  
for(int k = 0; k < 120; k++){
    for(int j = 0; j < 160; j++){
          ymax = 0;
          i2 = 0;
          
          if(my_array[j][k][0] == 0xFF)
             {
                
                ymin = k;
                xmin = j;
                xmax = j;
                
                while(my_array[j+i1][k+i2][0]==0xFF){my_array[j+i1][k+i2][0]=0x00;i1++;num1++;}               
             
                
                
                i1=0;i2++;
                
                
                
                while(my_array[j+i1][k+i2][0]==0xFF){
               
                while(my_array[j+i1][k+i2][0]==0xFF){i1--;}            
              
                
                if(xmin > j+i1)xmin =j+i1; 
                i1++;
                
                while(my_array[j+i1][k+i2][0]==0xFF){my_array[j+i1][k+i2][0]=0x00;i1++;num1++;}               
                
                if(xmax < j+i1)xmax =j+i1;
                
                i1=0;i2++;
                
                
               
                
                
                }
                
                ymax = k + i2;
                
                if(num1 != 0 && num1 > 500){
                
                xmax--;
                xmin++;
                
                center1_x = xmin + (xmax-xmin)/2;
                center1_y = ymin + (ymax-ymin)/2;
                
                *cent_x = center1_x;
                *cent_y = center1_y;
                
                
                my_array[xmax][center1_y][1]=0x7C;
                my_array[xmax][center1_y][0]=0x00;
                
                my_array[center1_x][ymax][1]=0x7C;
                my_array[center1_x][ymax][0]=0x00;
                
                my_array[xmin][center1_y][1]=0x7C;
                my_array[xmin][center1_y][0]=0x00;
                
                my_array[center1_x][ymin][1]=0x7C;
                my_array[center1_x][ymin][0]=0x00;
                
                
                my_array[center1_x][center1_y+2][1]=0x7C;
                my_array[center1_x][center1_y+2][0]=0x00;
                
                my_array[center1_x+2][center1_y+1][1]=0x7C;
                my_array[center1_x+2][center1_y+1][0]=0x00;
                
                my_array[center1_x+4][center1_y+2][1]=0x7C;
                my_array[center1_x+4][center1_y+2][0]=0x00;
                
                my_array[center1_x+1][center1_y+2][1]=0x7C;
                my_array[center1_x+1][center1_y+2][0]=0x00;
                
                my_array[center1_x+2][center1_y+2][1]=0x7C;
                my_array[center1_x+2][center1_y+2][0]=0x00;
                
                my_array[center1_x+3][center1_y+2][1]=0x7C;
                my_array[center1_x+3][center1_y+2][0]=0x00;
                
                my_array[center1_x+2][center1_y][1]=0x7C;
                my_array[center1_x+2][center1_y][0]=0x00;
                
                my_array[center1_x+2][center1_y+3][1]=0x7C;
                my_array[center1_x+2][center1_y+3][0]=0x00;
                
                my_array[center1_x+2][center1_y+4][1]=0x7C;
                my_array[center1_x+2][center1_y+4][0]=0x00;
                
                
                
                ymin = 0; ymax = 0; xmin = 0; xmax = 0;
                
                
                
                
                }
                
                num1 = 0;
             }}}  

}


void filter6(int *area, int *perimeter, unsigned char my_array[][120][2]){
  
  int ar = 0, per = 0;
  
  for(int k = 1; k < 119; k++){
  for(int j = 1; j < 159; j++){
          

           if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF))  
              {

           my_array[j][k][1] = 0x00;
           my_array[j][k][0] = 0x1F;
           per++;  

           for(int i = j+1; i < 159; i++)
           {
             
             if((my_array[i][k][1] != 0x7F) && (my_array[i][k][0] != 0xFF)){
               
             my_array[i][k][1] = 0x00;
             my_array[i][k][0] = 0x1F;
             per++; 
             break;
             }
           
           }
           break;
           }
            
          }} 
  
  for(int k = 1; k < 119; k++){
  for(int j = 1; j < 159; j++){
    if((my_array[j][k][1] == 0x7F) && (my_array[j][k][0] == 0xFF)){ar++;}
  }}
  
  
 *area = ar;
 *perimeter = per; 
  
}

void color_to_rgb(unsigned char *R, unsigned char *G, unsigned char *B, unsigned char col1, unsigned char col2){
  *R = col1 >> 2;
  *G = ((col1 & 0x03) << 3) + (col2 >> 5);
  *B = col2 & 0x1F;
}


unsigned int color_in_16bit(unsigned char col1, unsigned char col2){
  int RR = col1 >> 2;
  int GG = ((col1 & 0x03) << 3) + (col2 >> 5);
  int BB = col2 & 0x1F;
  
  int R1 = RR << 11; 
  int G1 = (GG << 1)+1;
  G1 = G1 << 5;
  
  return R1+G1+BB;
}


void Key_GPIO_Init(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
   // Enable GPIOC  clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}


void USART1_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1 ,ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //ÍÆÍìÊä³ö-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //¸¡¿ÕÊäÈë-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

   USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
    /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);
  
    /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  
    /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
  




  
}
/*******************************************************************************
* Function Name  : Get_HSEStartUpStatus
* Description    : Returns the HSE StartUp Status.
* Input          : None
* Output         : None
* Return         : HSE StartUp Status.
*******************************************************************************/
ErrorStatus Get_HSEStartUpStatus(void)
{
  return (HSEStartUpStatus);
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */



  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


#include "delay.h"

void delay_us(unsigned int i)
 {     
    while( i--)
       {	
        
        }
 }		  


void delay_ms(unsigned int time)
	 {
	  while(time--)
	  	  {	
		   delay_us(1000);
		   ;
		  }
	 }


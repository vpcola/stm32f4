/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "stdio.h"
#include "diskio.h"
#include "sdcard.h"
#include "Include.h"
/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

#define ATA		0
#define MMC		1
#define USB		2
#define SECTOR_SIZE 		512U /* Block Size in Bytes */

/*******************************************************************************
* Function Name  : SD_Insert_Serve 
* Description    : SD card insert or out serve                 
* Input          : None
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
void SD_Insert_Serve(void)
{	
	
	if( SD_nCD_STATUS() == SD_NOT_IN )
	{
		gEquipStatus.SD_status = STA_NODISK;						
	}
	else if( SD_nCD_STATUS() == SD_YES_IN )
	{					
		gEquipStatus.SD_status = disk_initialize(0);
		if(gEquipStatus.SD_status == STA_OK)
		{
			//if(TimerExist(T_WORK))	{ gEquipStatus.SD_status = STA_OK_T; }
			//if(SD_WP_STATUS() == SD_YES_WP)	{ gEquipStatus.SD_status = STA_PROTECT; }						
		}								
	}

}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	SD_Error state;	
	SD_CardInfo SDCardInfo;

	//if (drv)
    //{    
    //    return STA_PARERR;  //仅支持单磁盘操作，否则返回参数错误
    //}

	//if(no medium);
	
	state = SD_Init();

    
	if (state == SD_OK)	{
		    //----------------- Read CSD/CID MSD registers ------------------
		state = SD_GetCardInfo(&SDCardInfo);
	}else	{
	  	return STA_NOINIT;  //其他错误：初始化失败
	}
		  
	if (state == SD_OK)	{
	//----------------- Select Card --------------------------------
		state = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
	}else{
	  	return STA_NOINIT;  //其他错误：初始化失败
	} 
	
	if (state == SD_OK)
	{
	    state = SD_EnableWideBusOperation(SDIO_BusWide_4b);
	}
	else
	{
		return STA_NOINIT;  //其他错误：初始化失败
	}	  
	// Set Device Transfer Mode to DMA 
	if (state == SD_OK)
	{  
		state = SD_SetDeviceMode(SD_DMA_MODE);
	 	//state = SD_SetDeviceMode(SD_POLLING_MODE);
	    //state = SD_SetDeviceMode(SD_INTERRUPT_MODE);		
	}else{
	  	return STA_NOINIT;  //其他错误：初始化失败
	}
	
	if (state == SD_OK)
	{
	  	return STA_OK;           //初始化成功
	}
   
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	if(drv)
    {
        return STA_NOINIT;  //仅支持磁盘0操作
    }

    //检查SD卡是否插入
   // if(!SD_DET())
   // {
   //     return STA_NODISK;
   // }
    return 0;
//	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	SD_Error res = SD_OK;
    if (drv || !count)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }
    //if(!SD_DET())
   // {
    //    return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
   // }

	if(count==1)
    {
		res = SD_ReadBlock(sector << 9 ,(u32 *)(&buff[0]),SECTOR_SIZE);        
	}
	else
    {
		res =  SD_ReadMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),SECTOR_SIZE,count);        
	} 
	
	if(res == SD_OK)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }  
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	SD_Error res = SD_OK;

    if (drv || !count)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }

	if(count==1)
    {          
    	res =SD_WriteBlock(sector << 9 ,(u32 *)(&buff[0]),SECTOR_SIZE);
	}
	else
    {          
    	res = SD_WriteMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),SECTOR_SIZE,count);
	}
        
  	if(res == SD_OK)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }  
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}

DWORD get_fattime (void)
{
	//struct tm t;
   // DWORD fattime;
  //  t = Time_GetCalendarTime();
   // t.tm_year -= 1980;		//年份改为1980年起
   // t.tm_mon++;         	//0-11月改为1-12月
   // t.tm_sec /= 2;      	//将秒数改为0-29
    
  //  fattime = 0;
  //  fattime = (t.tm_year << 25)|(t.tm_mon<<21)|(t.tm_mday<<16)|\
              (t.tm_hour<<11)|(t.tm_min<<5)|(t.tm_sec);

    return 0;
}


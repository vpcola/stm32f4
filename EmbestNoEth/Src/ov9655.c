#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "ov9655.h"
#include "dcmi.h"
#include "i2c.h"
#include "dma.h"
#include "FreeRTOS.h"


#define I2C_CAM_TIMEOUT 1000
#define TIMEOUT         10

uint8_t SingleRandomWrite(uint8_t Device, uint16_t Addr, uint8_t Data)
{
    if (HAL_I2C_Mem_Write(&hi2c1, 
                Device,
                (uint8_t) Addr, // register address
                1, // sizeof (Addr)
                &Data,
                sizeof(Data),
                I2C_CAM_TIMEOUT ) == HAL_OK)
        return 0x00;
    else
        return 0xFF;
}

uint8_t SingleRandomRead(uint8_t Device, uint16_t Addr)
{
    uint8_t data = 0;

    if (HAL_I2C_Mem_Read(&hi2c1,
                Device,
                (uint8_t) Addr,
                1, // sizeof(uint8_t)
                &data,
                sizeof(data),
                I2C_CAM_TIMEOUT) == HAL_OK)
        return data;
    else
        return 0xFF;
}

void nDelay(uint32_t n)
{
    int i;
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
    {
        // delay in MS
        osDelay(n);
    }else
    {
        for(i = 0; i < n; i++);
    }
}


int OV9655_SetupCamera(CapImageSize size, ImageDestination dest)
{
  /* Reset and check the presence of the OV9655 camera module */
  if (SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x80))
  {
     return (0xFF);
  }

  /* OV9655 Camera size setup */
  if (size == QVGA)	
		OV9655_QVGASizeSetup();
	else
		OV9655_QQVGASizeSetup();

  /* Set the RGB565 mode */
  SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, 0x63);
  SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, 0x10);

  /* Invert the HRef signal*/
  SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM10, 0x08);

  /* Configure the DCMI to interface with the OV9655 camera module */
	if (dest == ToMemory)
		DCMI_DMASetup(DMA_MINC_ENABLE, DMA_MDATAALIGN_HALFWORD, DMA_NORMAL);
	else // To LCD
		DCMI_DMASetup(DMA_MINC_DISABLE, DMA_MDATAALIGN_HALFWORD, DMA_CIRCULAR);
  
  return (0x00);	
}

/**
 * @brief  Reset the OV9655 SCCB registers.
 * @param  None
 * @retval None
 */
void OV9655_Reset(void)
{
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, SCCB_REG_RESET);    
}

/**
 * @brief  Set the QVGA size(240*320).
 * @param  None
 * @retval None
 */
void OV9655_QVGASizeSetup(void)
{  
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x00, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x01, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x02, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x03, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x04, 0x03);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x09, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0b, 0x57);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0e, 0x61);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0f, 0x40);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x11, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x62);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x13, 0xc7);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x14, 0x3a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x16, 0x24);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x17, 0x18);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x18, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x19, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1a, 0x81);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1e, 0x00); /*0x20*/
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x24, 0x3c);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x25, 0x36);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x26, 0x72);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x27, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x28, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x29, 0x15);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2a, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2b, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2c, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x32, 0x12); // 0x32, 0xa4 for half res
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x33, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x34, 0x3f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x35, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x36, 0x3a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x38, 0x72);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x39, 0x57);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3a, 0xcc);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3b, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3d, 0x99);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3e, 0x02);  //0x3e, 0x0e for half res
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3f, 0xc1);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x40, 0xc0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x41, 0x41);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x42, 0xc0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x43, 0x0a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x44, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x45, 0x46);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x46, 0x62);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x47, 0x2a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x48, 0x3c);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4a, 0xfc);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4b, 0xfc);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4c, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4d, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4e, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4f, 0x98);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x50, 0x98);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x51, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x52, 0x28);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x53, 0x70);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x54, 0x98);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x58, 0x1a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x59, 0x85);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5a, 0xa9);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5b, 0x64);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5c, 0x84);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5d, 0x53);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5e, 0x0e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5f, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x60, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x61, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x62, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x63, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x64, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x65, 0x20);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x66, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x69, 0x0a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6b, 0x5a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6c, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6d, 0x55);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6e, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6f, 0x9d);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x70, 0x21);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x71, 0x78);
    nDelay(TIMEOUT);
    nDelay(TIMEOUT);  
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x72, 0x11); // 0x72, 0x22 for half res
    nDelay(TIMEOUT);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x73, 0x01); // 0x73, 0x02 for half res
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x74, 0x10); 
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x75, 0x10); 
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x76, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x77, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7A, 0x12);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7B, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7C, 0x16);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7D, 0x30);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7E, 0x5e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7F, 0x72);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x80, 0x82);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x81, 0x8e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x82, 0x9a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x83, 0xa4);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x84, 0xac);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x85, 0xb8);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x86, 0xc3);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x87, 0xd6);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x88, 0xe6);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x89, 0xf2);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8a, 0x24);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8c, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x90, 0x7d);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x91, 0x7b);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9d, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9e, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9f, 0x7a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa0, 0x79);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa1, 0x40);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa4, 0x50);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa5, 0x68);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa6, 0x4a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa8, 0xc1);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa9, 0xef);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaa, 0x92);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xab, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xac, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xad, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xae, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaf, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb2, 0xf2);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb3, 0x20);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb4, 0x20);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb5, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbb, 0xae);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbc, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbd, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbe, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc0, 0xaa);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc1, 0xc0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc2, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc3, 0x4e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc6, 0x05);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc7, 0x81); // 0xc7, 0x82 for half res
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc9, 0xe0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xca, 0xe8);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcb, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcc, 0xd8);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcd, 0x93);
    nDelay(TIMEOUT);
}

/**
 * @brief  Set the QQVGA size(120*160).
 * @param  None
 * @retval None
 */
void OV9655_QQVGASizeSetup(void)
{
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x00, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x01, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x02, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x03, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x04, 0x03);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x09, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0b, 0x57);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0e, 0x61);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0f, 0x40);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x11, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x62);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x13, 0xc7);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x14, 0x3a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x16, 0x24);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x17, 0x18);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x18, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x19, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1a, 0x81);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1e, 0x20);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x24, 0x3c);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x25, 0x36);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x26, 0x72);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x27, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x28, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x29, 0x15);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2a, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2b, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2c, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x32, 0xa4);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x33, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x34, 0x3f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x35, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x36, 0x3a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x38, 0x72);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x39, 0x57);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3a, 0xcc);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3b, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3d, 0x99);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3e, 0x0e); 
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3f, 0xc1);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x40, 0xc0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x41, 0x41);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x42, 0xc0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x43, 0x0a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x44, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x45, 0x46);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x46, 0x62);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x47, 0x2a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x48, 0x3c);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4a, 0xfc);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4b, 0xfc);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4c, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4d, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4e, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4f, 0x98);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x50, 0x98);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x51, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x52, 0x28);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x53, 0x70);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x54, 0x98);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x58, 0x1a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x59, 0x85);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5a, 0xa9);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5b, 0x64);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5c, 0x84);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5d, 0x53);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5e, 0x0e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5f, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x60, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x61, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x62, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x63, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x64, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x65, 0x20);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x66, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x69, 0x0a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6b, 0x5a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6c, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6d, 0x55);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6e, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6f, 0x9d);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x70, 0x21);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x71, 0x78);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x72, 0x22); 
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x73, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x74, 0x10);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x75, 0x10); 
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x76, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x77, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7A, 0x12);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7B, 0x08);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7C, 0x16);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7D, 0x30);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7E, 0x5e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7F, 0x72);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x80, 0x82);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x81, 0x8e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x82, 0x9a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x83, 0xa4);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x84, 0xac);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x85, 0xb8);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x86, 0xc3);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x87, 0xd6);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x88, 0xe6);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x89, 0xf2);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8a, 0x24);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8c, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x90, 0x7d);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x91, 0x7b);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9d, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9e, 0x02);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9f, 0x7a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa0, 0x79);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa1, 0x40);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa4, 0x50);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa5, 0x68);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa6, 0x4a);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa8, 0xc1);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa9, 0xef);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaa, 0x92);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xab, 0x04);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xac, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xad, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xae, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaf, 0x80);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb2, 0xf2);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb3, 0x20);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb4, 0x20);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb5, 0x00);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbb, 0xae);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbc, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbd, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbe, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc0, 0xaa);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc1, 0xc0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc2, 0x01);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc3, 0x4e);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc6, 0x05);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc7, 0x82);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc9, 0xe0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xca, 0xe8);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcb, 0xf0);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcc, 0xd8);
    nDelay(TIMEOUT);
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcd, 0x93);
    nDelay(TIMEOUT);
}

/**
 * @brief  Read the OV9655 Manufacturer identifier.
 * @param  OV9655ID: pointer to the OV9655 Manufacturer identifier. 
 * @retval None
 */
void OV9655_ReadID(OV9655_IDTypeDef* OV9655ID)
{
    OV9655ID->Manufacturer_ID1 = SingleRandomRead(OV9655_DEVICE_READ_ADDRESS, OV9655_MIDH);
    OV9655ID->Manufacturer_ID2 = SingleRandomRead(OV9655_DEVICE_READ_ADDRESS, OV9655_MIDL);
    OV9655ID->Version = SingleRandomRead(OV9655_DEVICE_READ_ADDRESS, OV9655_VER);
    OV9655ID->PID = SingleRandomRead(OV9655_DEVICE_READ_ADDRESS, OV9655_PID);
}

/**
 * @brief  Set the Internal Clock Prescaler.
 * @param  OV9655_Prescaler: the new value of the prescaler. 
 *         This parameter can be a value between 0x0 and 0x1F
 * @retval None
 */
void OV9655_SetPrescaler(uint8_t OV9655_Prescaler)
{
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_CLKRC, OV9655_Prescaler);
}

/**
 * @brief  Select the Output Format.
 * @param  OV9655_OuputFormat: the Format of the ouput Data.  
 *         This parameter can be one of the following values:
 *           @arg OUTPUT_FORMAT_RAWRGB_DATA 
 *           @arg OUTPUT_FORMAT_RAWRGB_INTERP    
 *           @arg OUTPUT_FORMAT_YUV              
 *           @arg OUTPUT_FORMAT_RGB    
 * @retval None
 */
void OV9655_SelectOutputFormat(uint8_t OV9655_OuputFormat)
{
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_OuputFormat);
}

/**
 * @brief  Select the Output Format Resolution.
 * @param  OV9655_FormatResolution: the Resolution of the ouput Data. 
 *         This parameter can be one of the following values:
 *           @arg FORMAT_CTRL_15fpsVGA 
 *           @arg FORMAT_CTRL_30fpsVGA_NoVArioPixel    
 *           @arg FORMAT_CTRL_30fpsVGA_VArioPixel     
 * @retval None
 */
void OV9655_SelectFormatResolution(uint8_t OV9655_FormatResolution)
{
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_FormatResolution);
}

/**
 * @brief  Set the new value of OV9655 registers
 * @param  OV9655_Register: the OV9655 Register to be configured. 
 * @param  Register_Val: The new value to be set 
 * @retval None
 */
void OV9655_SetRegister(uint8_t OV9655_Register, uint8_t Register_Val)
{
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_Register, Register_Val);
}

/**
 * @brief  Select the HREF Control signal option
 * @param  OV9665_HREFControl: the HREF Control signal option.
 *         This parameter can be one of the following value:
 *           @arg OV9665_HREFControl_Opt1: HREF edge offset to data output. 
 *           @arg OV9665_HREFControl_Opt2: HREF end 3 LSB    
 *           @arg OV9665_HREFControl_Opt3: HREF start 3 LSB      
 * @retval None
 */
void OV9655_HREFControl(uint8_t OV9665_HREFControl)
{
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_HREF, OV9665_HREFControl);
}

/**
 * @brief  Select the RGB format option
 * @param  OV9665_RGBOption: the RGB Format option.
 *         This parameter can be one of the following value:
 *           @arg RGB_NORMAL
 *           @arg RGB_565  
 *           @arg RGB_555    
 * @retval None
 */
void OV9655_SelectRGBOption(uint8_t OV9665_RGBOption)
{
    SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, OV9665_RGBOption);
}



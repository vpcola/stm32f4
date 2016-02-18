CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c STM32F4xx_HAL_Driver/Src 
vpath %.c CMSIS/Device/ST/STM32F4xx/Source/Templates
vpath %.c CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc

CFLAGS  = -g -O2 -Wall

# Define the device we are using
CFLAGS += -DSTM32F405xx -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))"
CFLAGS += -DUSE_HAL_DRIVER

CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffreestanding #-nostdlib 
CFLAGS += -I../Inc -I./CMSIS/Device/ST/STM32F4xx/Include 
CFLAGS += -I./CMSIS/Include -I./STM32F4xx_HAL_Driver/Inc
CFLAGS += -I./STM32F4xx_HAL_Driver/Inc/Legacy

SRCS = system_stm32f4xx.c \
       stm32f4xx_hal_adc_ex.c \
       stm32f4xx_hal_adc.c \
       stm32f4xx_hal_cortex.c \
       stm32f4xx_hal_dcmi_ex.c \
       stm32f4xx_hal_dcmi.c \
       stm32f4xx_hal_dma_ex.c \
       stm32f4xx_hal_dma.c \
       stm32f4xx_hal_eth.c \
       stm32f4xx_hal_flash_ex.c \
       stm32f4xx_hal_flash_ramfunc.c \
       stm32f4xx_hal_flash.c \
       stm32f4xx_hal_gpio.c \
       stm32f4xx_hal_i2c_ex.c \
       stm32f4xx_hal_i2c.c \
       stm32f4xx_hal_pwr_ex.c \
       stm32f4xx_hal_pwr.c \
       stm32f4xx_hal_rcc_ex.c \
       stm32f4xx_hal_rcc.c \
       stm32f4xx_hal_sd.c \
       stm32f4xx_hal_spi.c \
       stm32f4xx_hal_sram.c \
       stm32f4xx_hal_tim_ex.c \
       stm32f4xx_hal_tim.c \
       stm32f4xx_hal_uart.c \
       stm32f4xx_hal.c \
       stm32f4xx_ll_fsmc.c \
       stm32f4xx_ll_sdmmc.c


OBJS = $(SRCS:.c=.o)

.PHONY: libstm32f4.a

all: libstm32f4.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f4.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32f4.a

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################
vpath %.c ./src ./src/drivers

CFLAGS  = -g -O2 -Wall

CFLAGS += -DSTM32F407xx

CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffreestanding #-nostdlib 
CFLAGS += -I../../../Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include/
CFLAGS += -I../../../Drivers/CMSIS/Include
CFLAGS += -I../FreeRTOS/Source/CMSIS_RTOS
CFLAGS += -I../FreeRTOS/Source/include
CFLAGS += -I../FreeRTOS/Source/portable/GCC/ARM_CM4F
CFLAGS += -I./src -I./src/drivers

SRCS = ff.c \
	diskio.c \
	sd_diskio.c \
	ff_gen_drv.c 


OBJS = $(SRCS:.c=.o)

.PHONY: libfatfs.a

all: libfatfs.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libfatfs.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libfatfs.a

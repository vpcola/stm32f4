# put your *.o targets here, make should handle the rest!

SRCS =  adc.c \
	freertos.c \
	gpio.c \
	guided.c \
	i2c.c \
	main.c \
	motor.c \
	newlibstubs.c \
	rtosheap.c \
	shell.c \
	stm32f4xx_hal_msp.c \
	stm32f4xx_it.c \
	tim.c \
	usart.c \
	utility.c


# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

PROJ_NAME=main

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
OBJSIZE=arm-none-eabi-size

CFLAGS  = -g -O2 -Wall -TSTM32F405RGTx_FLASH.ld
# Define the device we are using
CFLAGS += -DSTM32F405xx -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))"
CFLAGS += -DUSE_HAL_DRIVER

CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
 
ODFLAGS	= -x
LDFLAGS += -Wl,-Map,$(PROJ_NAME).map

###################################################

vpath %.c Src
vpath %.a Drivers
vpath %.a Middlewares/Third_Party/FreeRTOS

ROOT=$(shell pwd)

CFLAGS += -IInc 
CFLAGS += -IDrivers/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -IDrivers/CMSIS/Include
CFLAGS += -IDrivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy
CFLAGS += -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
CFLAGS += -IMiddlewares/Third_Party/FreeRTOS/Source/Include
CFLAGS += -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F

LIBS = -LDrivers -lstm32f4
LIBS += -LMiddlewares/Third_Party/FreeRTOS -lfreertos

	
SRCS += Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f405xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: Drivers FreeRTOS proj

all: Drivers FreeRTOS proj

Drivers:
	$(MAKE) -C Drivers
	
FreeRTOS:
	$(MAKE) -C Middlewares/Third_Party/FreeRTOS

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(LDFLAGS)
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -x $(PROJ_NAME).elf > $(PROJ_NAME).dmp
	@echo " "
	@$(OBJSIZE) -d $(PROJ_NAME).elf

clean:
	$(MAKE) -C Drivers clean
	$(MAKE) -C Middlewares/Third_Party/FreeRTOS clean
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).dmp
	rm -f $(PROJ_NAME).map

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c ./Source ./Source/portable/MemMang ./Source/CMSIS_RTOS
vpath %.c ./Source/portable/GCC/ARM_CM4F

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffreestanding #-nostdlib 
CFLAGS += -I../../../Drivers/CMSIS/Include
CFLAGS += -I../../../Inc 
CFLAGS += -I./Source -I./Source/include -I./Source/portable/GCC/ARM_CM4F -I./Source/CMSIS_RTOS

SRCS = port.c \
       list.c \
       queue.c \
       tasks.c \
       event_groups.c \
       timers.c \
       heap_4.c \
       cmsis_os.c 

       
OBJS = $(SRCS:.c=.o)

.PHONY: libfreertos.a

all: libfreertos.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libfreertos.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libfreertos.a

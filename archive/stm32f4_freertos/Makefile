PROJECT = cart

EXECUTABLE = $(PROJECT).elf
BIN_IMAGE  = $(PROJECT).bin
HEX_IMAGE  = $(PROJECT).hex

# Set the path to STM32F429I-Discovery firmware package
STDP ?= ../STM32F429I-Discovery_FW_V1.0.1

# Toolchain configurations
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size
GDB = $(CROSS_COMPILE)gdb

# Optimization level from 0 to 5
OPTLVL := 0

# Cortex-M4 implements the ARMv7E-M architecture
CPU     = cortex-m4
CFLAGS  = -O$(OPTLVL) -ggdb -Wall
CFLAGS += -mcpu=$(CPU) -march=armv7e-m -mtune=cortex-m4
CFLAGS += -mlittle-endian -mthumb
#CFLAGS += -g -std=c99 # -std=c99 doesn't work with syscalls.c

# Advanced configurations
CFLAGS += -ffast-math
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections
CFLAGS += -fno-common
CFLAGS += --param max-inline-insns-single=1000
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

# Specify STM32F429
CFLAGS += -DSTM32F429_439xx
# To run from FLASH
CFLAGS += -DVECT_TAB_FLASH
# STM32F4xx_StdPeriph_Driver
CFLAGS += -DUSE_STDPERIPH_DRIVER
#CFLAGS += -D"assert_param(expr)=((void)0)"
CFLAGS += -DHSE_VALUE=8000000 -D__FPU_PRESENT=1 -D__FPU_USED=1 -DARM_MATH_CM4

# Print directories of libc.a and libgcc.a when linker runs later
define get_library_path
    $(shell dirname $(shell $(CC) $(CFLAGS) -print-file-name=$(1)))
endef
LDFLAGS += -L $(call get_library_path,libc.a)
LDFLAGS += -L $(call get_library_path,libgcc.a)
LDFLAGS += -T $(PWD)/CORTEX_M4F_STM32F4/stm32f429zi_flash.ld

# STARTUP FILE
OBJS += $(PWD)/CORTEX_M4F_STM32F4/startup_stm32f429_439xx.o

OBJS += \
    $(PWD)/src/cart.o \
    $(PWD)/CORTEX_M4F_STM32F4/startup/system_stm32f4xx.o \
    #$(PWD)/CORTEX_M4F_STM32F4/stm32f4xx_it.o \

OBJS += \
    $(PWD)/FreeRTOS/croutine.o \
    $(PWD)/FreeRTOS/event_groups.o \
    $(PWD)/FreeRTOS/list.o \
    $(PWD)/FreeRTOS/queue.o \
    $(PWD)/FreeRTOS/tasks.o \
    $(PWD)/FreeRTOS/timers.o \
    $(PWD)/FreeRTOS/portable/GCC/ARM_CM4F/port.o \
    $(PWD)/FreeRTOS/portable/MemMang/heap_1.o \

OBJS += \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/misc.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma2d.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_ltdc.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rng.o \
    $(PWD)/CORTEX_M4F_STM32F4/Libraries/syscall/syscalls.o \
    $(PWD)/Utilities/STM32F429I-Discovery/stm32f429i_discovery.o \
    $(PWD)/Utilities/STM32F429I-Discovery/stm32f429i_discovery_sdram.o \
    $(PWD)/Utilities/STM32F429I-Discovery/stm32f429i_discovery_lcd.o \
    $(PWD)/Utilities/STM32F429I-Discovery/stm32f429i_discovery_ioe.o

# Cart TODO
#OBJS += \

CFLAGS += -I$(PWD)/include \
    -I$(PWD)/CORTEX_M4F_STM32F4 \
    -I$(PWD)/FreeRTOS/include \
    -I$(PWD)/FreeRTOS/portable/GCC/ARM_CM4F \
    -I$(PWD)/CORTEX_M4F_STM32F4/board \
    -I$(PWD)/CORTEX_M4F_STM32F4/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
    -I$(PWD)/CORTEX_M4F_STM32F4/Libraries/CMSIS/Include \
    -I$(PWD)/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver/inc \
    -I$(PWD)/Utilities/STM32F429I-Discovery

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@
	$(OBJCOPY) -O ihex $^ $(HEX_IMAGE)
	$(OBJDUMP) -h -S -D $(EXECUTABLE) > $(PROJECT).lst
	$(SIZE) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJS)
	$(LD) -o $@ $(OBJS) \
		--start-group $(LIBS) --end-group \
		$(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: flash clean openocd_flash debug

flash:
	st-flash write $(BIN_IMAGE) 0x8000000

openocd_flash:
	openocd \
	-f board/stm32f429discovery.cfg \
	-c "init" \
	-c "reset init" \
	-c "flash probe 0" \
	-c "flash info 0" \
	-c "flash write_image erase $(BIN_IMAGE)  0x08000000" \
	-c "reset run" -c shutdown

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -rf $(HEX_IMAGE)
	rm -f $(OBJS)
	rm -f $(PROJECT).lst

debug:
	$(GDB) -tui -ex "tar ext :4242" $(EXECUTABLE) # start st-util first

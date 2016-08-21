# Segway Robot's Makefile

# 	Modified from the makefile of the  crazyflie project
#			https://github.com/bitcraze/crazyflie-firmware/blob/master/Makefile
#
# 		CrazyFlie's Makefile
# 		Copyright (c) 2011,2012 Bitcraze AB

# 	This Makefile compiles all the objet file to ./bin/ and the resulting firmware
# 	image in ./cflie.elf and ./cflie.bin

# Put your personal build config in tools/make/config.mk and DO NOT COMMIT IT!
# Make a copy of tools/make/config.mk.example to get you started
#-include tools/make/config.mk

#CFLAGS += $(EXTRA_CFLAGS)

######### JTAG and environment configuration ##########
OPENOCD           ?= openocd
OPENOCD_INTERFACE ?= interface/stlink-v2.cfg
OPENOCD_CMDS      ?=
CROSS_COMPILE     ?= arm-none-eabi-
PYTHON2           ?= python2
DFU_UTIL          ?= dfu-util
DEBUG             ?= 0
PLATFORM          ?= STM32F4

######### Stabilizer configuration ##########
##### Sets the name of the stabilizer module to use.
SENSORS            ?= stock
ESTIMATOR          ?= complementary
CONTROLLER         ?= pid
#POWER_DISTRIBUTION ?= stock


ifeq ($(PLATFORM), STM32F1)
OPENOCD_TARGET    ?= target/stm32f1x_stlink.cfg
USE_FPU            = 0
endif
ifeq ($(PLATFORM), STM32F4)
OPENOCD_TARGET    ?= target/stm32f4x_stlink.cfg
USE_FPU           ?= 1
endif


ifeq ($(PLATFORM), STM32F1)
REV               ?= F
endif
ifeq ($(PLATFORM), STM32F4)
# Now needed for SYSLINK
#CFLAGS += -DUSE_RADIOLINK_CRTP     # Set CRTP link to radio
#CFLAGS += -DENABLE_UART          # To enable the uart
REV               ?= D
endif

# OpenOCD conf
RTOS_DEBUG        ?= 0

############### Location configuration ################
FREERTOS = src/libraries/FreeRTOS/Source
ifeq ($(USE_FPU), 1)
PORT = $(FREERTOS)/portable/GCC/ARM_CM4F
else
PORT = $(FREERTOS)/portable/GCC/ARM_CM3
endif

ifeq ($(PLATFORM), STM32F1)
LINKER_DIR = scripts/stm32f103/
ST_OBJ_DIR  = scripts/stm32f103/
endif
ifeq ($(PLATFORM), STM32F4)
LINKER_DIR = scripts/stm32f429/
ST_OBJ_DIR  = scripts/stm32f429/
endif

# Location of libraries
STLIB = src/libraries

################ Build configuration ##################
# St Lib
VPATH_STM32F1 += $(STLIB)/CMSIS/Core/CM3
VPATH_STM32F1 += $(STLIB)/CMSIS/STM32F1xx/Source
VPATH_STM32F1 += $(STLIB)/STM32_CPAL_Driver/src
VPATH_STM32F1 += $(STLIB)/STM32_CPAL_Driver/devices/stm32f10x
CRT0_STM32F1 = startup_stm32f10x_md.o system_stm32f10x.o

VPATH_STM32F4 += $(STLIB)/CMSIS/STM32F4xx/Source/
VPATH_STM32F4 += $(STLIB)/STM32_CPAL_Driver/src
VPATH_STM32F4 += $(STLIB)/STM32_USB_Device_Library/Core/src
VPATH_STM32F4 += $(STLIB)/STM32_USB_OTG_Driver/src
VPATH_STM32F4 += $(STLIB)/STM32_CPAL_Driver/devices/stm32f4xx
VPATH_STM32F4 += $(STLIB)/STM32F429I-Discovery
CRT0_STM32F4 = startup_stm32f429_439xx.o system_stm32f4xx.o

# Build selected components of the STM32F standard peripheral library
-include $(ST_OBJ_DIR)/st_obj.mk

# Addtional library build targets
ST_OBJ_STM32F1 =  #cpal_hal.o cpal_i2c.o cpal_usercallback_template.o cpal_i2c_hal_stm32f10x.o
ST_OBJ_STM32F4 = cpal_hal.o cpal_i2c.o cpal_usercallback_template.o cpal_i2c_hal_stm32f4xx.o
# USB obj
#ST_OBJ_STM32F4 += usb_core.o usb_dcd_int.o usb_dcd.o
# USB Device obj
#ST_OBJ_STM32F4 += usbd_ioreq.o usbd_req.o usbd_core.o

# libdw dw1000 driver
#VPATH_STM32F4 += vendor/libdw1000/src

# FreeRTOS
VPATH += $(PORT)
PORT_OBJ = port.o
VPATH +=  $(FREERTOS)/portable/MemMang
MEMMANG_OBJ = heap_4.o

VPATH += $(FREERTOS)
FREERTOS_OBJ = list.o tasks.o queue.o timers.o $(MEMMANG_OBJ)

# Segway robot sources
# add additional source directories here
# VPATH: common source code directories
# VPATH_STM32F1/4: target specific source code directories
VPATH += src/modules/src src/drivers/src src/hal/src src/utils/src
VPATH_STM32F1 += src/platform/stm32f1
VPATH_STM32F4 += src/platform/stm32f4

ifeq ($(PLATFORM), STM32F1)
VPATH += $(VPATH_STM32F1)
endif
ifeq ($(PLATFORM), STM32F4)
VPATH += $(VPATH_STM32F4)
endif

############### Source files configuration ################

# Build objects
#		OBJ: combination of the following objects
#		- ST_OBJ: objects of the ST SPL
#		- PROJ_OBJ: common targets for the project
#		- FREERTOS_OBJ: objects of FreeRTOS implementation
#		- PORT_OBJ: port object for FreeRTOS
#		- CRT0_STM32F1/4: target specific objects for startup
#		- ST_OBJ_STM32F1/4: target specific objects from ST library
#		- PROJ_OBJ_STM32F1/4: target specific objects from user code

# Init
PROJ_OBJ += main.o
#PROJ_OBJ_STM32F1 += stm32f10x_it.o
# PROJ_OBJ_STM32F4 += stm32f4xx_it.o

# Drivers
#PROJ_OBJ_STM32F1 += led_f103.o i2cdev_f103.o i2croutines.o adc_f103.o mpu6050.o
#PROJ_OBJ_STM32F1 += hmc5883l.o ms5611.o nrf24l01.o eeprom.o watchdog.o
#PROJ_OBJ_STM32F1 += eskylink.o
PROJ_OBJ_STM32F4 += led_f429.o mpu6500.o i2cdev_f429.o #ws2812_cf2.o lps25h.o
PROJ_OBJ_STM32F4 += ak8963.o gpio.o motor.o pwm.o timer.o #eeprom.o
#PROJ_OBJ_STM32F4 += uart_syslink.o swd.o uart1.o uart2.o watchdog.o
#PROJ_OBJ_STM32F4 += cppm.o

# USB Files
#PROJ_OBJ_STM32F4 += usb_bsp.o usblink.o usbd_desc.o usb.o

# Hal
PROJ_OBJ += uart.o #crtp.o ledseq.o freeRTOSdebug.o buzzer.o
#PROJ_OBJ_STM32F1 += imu_cf1.o pm_f103.o nrf24link.o ow_none.o uart.o
PROJ_OBJ_STM32F4 += imu.o #pm_f405.o syslink.o radiolink.o ow_syslink.o proximity.o usec_time.o

# libdw
#PROJ_OBJ_STM32F4 += libdw1000.o libdw1000Spi.o

# Modules
PROJ_OBJ += system.o #comm.o console.o pid.o crtpservice.o param.o mem.o
#PROJ_OBJ += log.o worker.o trigger.o sitaw.o queuemonitor.o
#PROJ_OBJ_STM32F1 += sound_cf1.o
#PROJ_OBJ_STM32F4 += platformservice.o sound_cf2.o extrx.o

# Stabilizer modules
PROJ_OBJ += sensfusion6.o stabilizer.o #commander.o attitude_pid_controller.o
#PROJ_OBJ += position_estimator_altitude.o position_controller_pid.o
PROJ_OBJ += estimator.o #controller_$(CONTROLLER).o
PROJ_OBJ += sensors.o #power_distribution_$(POWER_DISTRIBUTION).o

# Utilities
PROJ_OBJ += eprintf.o filter.o #cpuid.o cfassert.o crc.o num.o debug.o
#PROJ_OBJ += version.o FreeRTOS-openocd.o
#PROJ_OBJ_STM32F1 += configblockflash.o
#PROJ_OBJ_STM32F4 += configblockeeprom.o

OBJ = $(FREERTOS_OBJ) $(PORT_OBJ) $(ST_OBJ) $(PROJ_OBJ)
ifeq ($(PLATFORM), STM32F1)
OBJ += $(CRT0_STM32F1) $(ST_OBJ_STM32F1) $(PROJ_OBJ_STM32F1)
endif
ifeq ($(PLATFORM), STM32F4)
OBJ += $(CRT0_STM32F4) $(ST_OBJ_STM32F4) $(PROJ_OBJ_STM32F4)
endif

ifdef P
  C_PROFILE = -D P_$(P)
endif

############### Compilation configuration ################
AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
GDB = $(CROSS_COMPILE)gdb

# common include directories for the whole project
INCLUDES  = -I$(FREERTOS)/include -I$(PORT) -Isrc
INCLUDES += -Isrc/modules/interface -Isrc/config -Isrc/hal/interface
INCLUDES += -Isrc/drivers/interface -Isrc/utils/interface -Isrc/platform
INCLUDES += -I$(STLIB)/CMSIS/Include

# target specific include directories
INCLUDES_STM32F1 += -Isrc/platform/stm32f1
INCLUDES_STM32F1 += -I$(STLIB)/STM32F10x_StdPeriph_Driver/inc
INCLUDES_STM32F1 += -I$(STLIB)/CMSIS/Core/CM3
INCLUDES_STM32F1 += -I$(STLIB)/CMSIS/STM32F1xx/Include
INCLUDES_STM32F1 += -I$(STLIB)/STM32_CPAL_Driver/inc
INCLUDES_STM32F1 += -I$(STLIB)/STM32_CPAL_Driver/devices/stm32f10x

INCLUDES_STM32F4 += -Isrc/platform/stm32f4
INCLUDES_STM32F4 += -I$(STLIB)/STM32F4xx_StdPeriph_Driver/inc
INCLUDES_STM32F4 += -I$(STLIB)/CMSIS/STM32F4xx/Include
INCLUDES_STM32F4 += -I$(STLIB)/STM32_CPAL_Driver/inc
INCLUDES_STM32F4 += -I$(STLIB)/STM32_CPAL_Driver/devices/stm32f4xx
INCLUDES_STM32F4 += -I$(STLIB)/STM32_USB_Device_Library/Core/inc
INCLUDES_STM32F4 += -I$(STLIB)/STM32_USB_OTG_Driver/inc
INCLUDES_STM32F4 += -I$(STLIB)/STM32F429I-Discovery

ifeq ($(USE_FPU), 1)
	PROCESSOR = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
	CFLAGS += -fno-math-errno
else
	ifeq ($(PLATFORM), STM32F1)
		PROCESSOR = -mcpu=cortex-m3 -mthumb
	endif
	ifeq ($(PLATFORM), STM32F4)
		PROCESSOR = -mcpu=cortex-m4 -mthumb
	endif
endif

# Flags required by the ST library
STFLAGS_STM32F1 = -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -include stm32f10x_conf.h -DPLATFORM_F1
STFLAGS_STM32F4 = -DSTM32F4XX -DSTM32F429_439xx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -DPLATFORM_F4

ifeq ($(DEBUG), 1)
  CFLAGS += -O0 -g3 -DDEBUG
else
  CFLAGS += -Os -g3
endif

ifeq ($(LTO), 1)
  CFLAGS += -flto
endif

#CFLAGS += -DBOARD_REV_$(REV)

CFLAGS += $(PROCESSOR) $(INCLUDES) $(STFLAGS)
ifeq ($(PLATFORM), STM32F1)
CFLAGS += $(INCLUDES_STM32F1) $(STFLAGS_STM32F1)
endif
ifeq ($(PLATFORM), STM32F4)
CFLAGS += $(INCLUDES_STM32F4) $(STFLAGS_STM32F4)
endif

CFLAGS += -Wall -fno-strict-aliasing $(C_PROFILE)
# Compiler flags to generate dependency files:
CFLAGS += -MD -MP -MF $(BIN)/dep/$(@).d -MQ $(@)
# Permits to remove un-used functions and global variables from output file
CFLAGS += -ffunction-sections -fdata-sections

ASFLAGS = $(PROCESSOR) $(INCLUDES)
LDFLAGS = --specs=nano.specs $(PROCESSOR) -Wl,-Map=$(PROG).map,--cref,--gc-sections,--undefined=uxTopUsedPriority

# Flags required by the ST library
ifeq ($(PLATFORM), STM32F1)
  LDFLAGS += -T $(LINKER_DIR)/stm32f103_flash.ld
  LOAD_ADDRESS = 0x8000000
else
  LDFLAGS += -T $(LINKER_DIR)/stm32f429zi_flash.ld
  LOAD_ADDRESS = 0x8000000
endif

ifeq ($(LTO), 1)
  LDFLAGS += -Os -flto -fuse-linker-plugin
endif

# Program name
ifeq ($(PLATFORM), STM32F1)
PROG = segway_stm32f1
else
PROG = segway_stm32f4
endif
#Where to compile the .o
BIN = bin
VPATH += $(BIN)

# Dependency files to include
DEPS := $(foreach o,$(OBJ),$(BIN)/dep/$(o).d)

##################### Misc. ################################
ifeq ($(SHELL),/bin/sh)
  COL_RED=\033[1;31m
  COL_GREEN=\033[1;32m
  COL_RESET=\033[m
endif

#################### Targets ###############################


all: build
build: compile print_version size
compile: $(PROG).hex $(PROG).bin #$(PROG).dfu

#clean_version:
#ifeq ($(SHELL),/bin/sh)
#	@echo "  CLEAN_VERSION"
#	@rm -f version.c
#endif

print_version: compile
ifeq ($(PLATFORM), STM32F1)
	@echo "Segway robot firmware for STM32F1 build!"
endif
ifeq ($(PLATFORM), STM32F4)
	@echo "Segway robot firmware for STM32F4 build!"
endif
	@$(PYTHON2) tools/python/versionTemplate.py --print-version


size: compile
	@$(SIZE) -B $(PROG).elf

#Flash the stm.
# flash:
#     $(OPENOCD) -d2 -f $(OPENOCD_INTERFACE) $(OPENOCD_CMDS) -f $(OPENOCD_TARGET) -c init -c targets -c "reset halt" \
#                  -c "flash write_image erase $(PROG).elf" -c "verify_image $(PROG).elf" -c "reset run" -c shutdown

flash_dfu:
	$(DFU_UTIL) -a 0 -D $(PROG).dfu

#STM utility targets
halt:
	$(OPENOCD) -d0 -f $(OPENOCD_INTERFACE) $(OPENOCD_CMDS) -f $(OPENOCD_TARGET) -c init -c targets -c "halt" -c shutdown

reset:
	$(OPENOCD) -d0 -f $(OPENOCD_INTERFACE) $(OPENOCD_CMDS) -f $(OPENOCD_TARGET) -c init -c targets -c "reset" -c shutdown

openocd:
	$(OPENOCD) -d2 -f $(OPENOCD_INTERFACE) $(OPENOCD_CMDS) -f $(OPENOCD_TARGET) -c init -c targets -c "\$$_TARGETNAME configure -rtos auto"

trace:
	$(OPENOCD) -d2 -f $(OPENOCD_INTERFACE) $(OPENOCD_CMDS) -f $(OPENOCD_TARGET) -c init -c targets -f tools/trace/enable_trace.cfg

gdb: $(PROG).elf
	$(GDB) -ex "target remote localhost:3333" -ex "monitor reset halt" $^

# In order to use JLink to load the code to Flash, you need the following 2 steps:
# 1. start a gdb server first: make start_gdb_server
# 2. load code and debug/run: make connect_gdb_server
start_gdb_server:
	JLinkGDBServer -if SWD -device STM32F103T8

connect_gdb_server:
	arm-none-eabi-gdb -x ./scripts/jlink.gdbinit $(PROG).elf

# flash and debug stm32f4
flash:
	st-flash write $(PROG).bin 0x8000000
debug:
	$(GDB) -tui -ex "tar ext :4242" $(PROG).elf # start st-util first

#Print preprocessor #defines
prep:
	@$(CC) $(CFLAGS) -dM -E - < /dev/null

#check_submodules:
#	@$(PYTHON2) tools/make/check-for-submodules.py

#include actual build targets
include targets.mk

#include dependencies
-include $(DEPS)

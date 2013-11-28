CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
SIZE=arm-none-eabi-size
OBJDUMP=arm-none-eabi-objdump
OBJCOPY=arm-none-eabi-objcopy
TOP_DIR=$(shell pwd)


_CFLAGS = $(CFLAGS)

ifndef $(OSC)
    $(warning "OSC value was not defined using default 16MHz, pass CFLAGS='-DOSC=xxMhz' to override")
    _CFLAGS += -DOSC=16
endif

ifneq ($(FLOAT_TYPE), hard)
    ifneq ($(FLOAT_TYPE), soft)
        FLOAT_TYPE = hard
    endif
endif

ifeq ($(DEBUG), 1)
_CFLAGS += -O0 -ggdb
else ifeq ($(DEBUG), 0)
_CFLAGS += -O2
else 
_CFLAGS += -O0 -ggdb
endif

_CFLAGS += -Wall -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4 -nostdlib

ifeq ($(FLOAT_TYPE), hard)
_CFLAGS += -fsingle-precision-constant -Wdouble-promotion
_CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
_CFLAGS += -msoft-float
endif

_CFLAGS += -DSTM32F40_41xxx

###################################################

#Export Variables
export CC
export AS
export LD
export AR
export SIZE
export OBJCOPY
export OBJDUMP
export _CFLAGS
export TOP_DIR
###################################################

all:
	$(MAKE) -C StdPeriph
	$(MAKE) -C USB_OTG
	$(MAKE) -C USB_Device
	$(MAKE) -C USB_Generic
	$(MAKE) -C Examples
#	$(MAKE) -C fat_fs 

clean:
	$(MAKE) clean -C StdPeriph
	$(MAKE) clean -C USB_OTG
	$(MAKE) clean -C USB_Device
	$(MAKE) clean -C USB_Generic
	$(MAKE) clean -C Examples
#	$(MAKE) clean -C fat_fs

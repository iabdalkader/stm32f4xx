CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
SIZE=arm-none-eabi-size
OBJDUMP=arm-none-eabi-objdump
OBJCOPY=arm-none-eabi-objcopy
TOP_DIR=$(shell pwd)


_CFLAGS = $(CFLAGS)

ifndef OSC
    $(warning "OSC value is not defined using default 12MHz, use make OSC=xx in Mhz to override")
    OSC=12
endif

ifneq ($(FLOAT_TYPE), hard)
    ifneq ($(FLOAT_TYPE), soft)
        FLOAT_TYPE = hard
    endif
endif

ifeq ($(DEBUG), 1)
    OPT_FLAGS = -g -O0
else
    OPT_FLAGS = -O2
endif

#ifeq ($(FLOAT_TYPE), hard)
#_CFLAGS += -fsingle-precision-constant -Wdouble-promotion
#_CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard -D__VFP_FP__
#else
#_CFLAGS += -msoft-float -D__SOFTFP__
#endif

_CFLAGS += -Wall $(OPT_FLAGS) -mlittle-endian -mthumb -nostartfiles -mcpu=cortex-m4 -mabi=aapcs-linux -fshort-enums
_CFLAGS += -fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_TYPE) -std=gnu99
_CFLAGS += -DOSC=$(OSC) -DSTM32F40_41xxx
          -I../include/MicroPython/py -I./py -DSTM32F40_41xxx -DUSE_USB_OTG_FS -DARM_MATH_CM4 -D__FPU_PRESENT  -DOSC=12

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
	$(MAKE) -C CMSIS
	$(MAKE) -C StdPeriph
	$(MAKE) -C USB_OTG
	$(MAKE) -C USB_Device
	$(MAKE) -C USB_Generic
	$(MAKE) -C Examples
	$(MAKE) -C FatFS
	$(MAKE) -C CC3000

clean:
	$(MAKE) clean -C CMSIS
	$(MAKE) clean -C StdPeriph
	$(MAKE) clean -C USB_OTG
	$(MAKE) clean -C USB_Device
	$(MAKE) clean -C USB_Generic
	$(MAKE) clean -C Examples
	$(MAKE) clean -C FatFS
	$(MAKE) clean -C CC3000

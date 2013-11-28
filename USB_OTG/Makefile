# Includes
 override CFLAGS = $(_CFLAGS) -Iinc -I../CMSIS -I$(TOP_DIR)/StdPeriph/inc -DUSE_USB_OTG_FS

# Sources
SRCS = src/usb_core.c src/usb_bsp.c  src/usb_dcd.c src/usb_dcd_int.c  

ifdef USB_HOST_MODE
SRCS += src/usb_hcd.c src/usb_hcd_int.c
endif 

ifdef USB_OTG_MODE
SRCS += src/usb_otg.c
endif

OBJS = $(SRCS:.c=.o)
LIBNAME = libusbcore.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.s.o :
	$(AS) $(AFLAGS) $< -o $@

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)

# Includes
override CFLAGS = $(_CFLAGS) -Iinc -I../CMSIS/inc -I$(TOP_DIR)/StdPeriph/inc -I$(TOP_DIR)/USB_OTG/inc -DUSE_USB_OTG_FS

# Sources
SRCS = $(wildcard src/*.c) 
OBJS = $(SRCS:.c=.o)
LIBNAME = libusbdevcore.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.s.o :
	$(AS) $(AFLAGS) $< -o $@

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)

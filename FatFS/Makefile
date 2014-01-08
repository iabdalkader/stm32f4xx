# Includes
override CFLAGS = $(_CFLAGS) -Iinc -I$(TOP_DIR)/StdPeriph/inc -I$(TOP_DIR)/CMSIS/inc

# Sources
SRCS = $(wildcard src/*.c) 
OBJS = $(SRCS:.c=.o)
LIBNAME = libfatfs.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.s.o :
	$(AS) $(AFLAGS) $< -o $@

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)

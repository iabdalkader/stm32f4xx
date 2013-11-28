# Includes
override CFLAGS = $(_CFLAGS) -Iinc -I../CMSIS

# Sources
SRCS = $(wildcard src/*.c) 
OBJS = $(SRCS:.c=.o) src/startup_stm32f4xx.o
LIBNAME = libstdperiph.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.s.o :
	$(AS) $(AFLAGS) $< -o $@

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)

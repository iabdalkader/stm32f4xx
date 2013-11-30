# Includes
override CFLAGS = $(_CFLAGS) -Iinc -DARM_MATH_CM4 -D__FPU_PRESENT

# Sources
SRCS = $(wildcard src/BasicMathFunctions/*.c) 
SRCS = $(wildcard src/CommonTables/*.c) 
SRCS = $(wildcard src/ComplexMathFunctions/*.c) 
SRCS = $(wildcard src/ControllerFunctions/*.c) 
SRCS = $(wildcard src/FastMathFunctions/*.c) 
SRCS = $(wildcard src/FilteringFunctions/*.c) 
SRCS = $(wildcard src/MatrixFunctions/*.c) 
SRCS = $(wildcard src/StatisticsFunctions/*.c) 
SRCS = $(wildcard src/SupportFunctions/*.c) 
SRCS = $(wildcard src/TransformFunctions/*.c) 
OBJS = $(SRCS:.c=.o) 
LIBNAME = libdsp.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.s.o :
	$(AS) $(AFLAGS) $< -o $@

$(LIBNAME): $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) $(LIBNAME)

override CFLAGS = $(_CFLAGS) -Iinc -I$(TOP_DIR)/StdPeriph/inc -I$(TOP_DIR)/CMSIS/inc

#Linker Flags
LDFLAGS = -mcpu=cortex-m4 -mthumb -mcpu=cortex-m4 -mthumb -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += -Wl,-Map=$(BIN).map -T$(TOP_DIR)/stm32f4xx.ld -L. -L$(TOP_DIR)/StdPeriph

# Sources
BIN  = "blinky"
SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c=.o) 
#Libraries
LIB = -lc -lm -lstdperiph

all:: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIB) -o $(BIN).elf
	$(OBJCOPY) -Oihex -j .text -j .data $(BIN).elf $(BIN).hex
	$(OBJCOPY) -Obinary  -j .text -j .data $(BIN).elf $(BIN).bin
	$(OBJDUMP) -d $(BIN).elf > $(BIN).dis

stats: $(BIN).elf
	$(SIZE) $(BIN).elf

clean:
	$(RM) *.o *.elf *.bin *.map *.hex *.dis

.c.o :
	$(CC) $(CFLAGS) -c $<

.s.o :
	$(AS) $(AFLAGS) $< -o $@

flash::
	dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D $(BIN).bin

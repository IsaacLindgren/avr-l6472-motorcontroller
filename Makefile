TARGET=MotorController
SRCS=$(wildcard *.c)

$(warning $(SRCS))

MMCU=atmega48a

CXX=avr-gcc
CFLAGS=-g -mmcu=$(MMCU) -Os

.PHONEY : all
.PHONEY : clean

all : $(TARGET)

clean :
	$(RM) *.o *.hex *.elf

$(TARGET) : $(SRCS)
	$(CXX) $(CFLAGS) -o $@.elf $^ 
	avr-objcopy -j .text -j .data -O ihex $@.elf $@.hex


.DEFAULT_GOAL := all
PATH ?= ./crc32
FILE_NAME ?= crc32
LDFLAGS = -static -lm
TARGET:=arm
CC_FLAGS = -O3


x86:CC=gcc
x86:CC_FLAGS = -Ofast -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm


A9:CC_FLAGS += -mcpu=cortex-a9 -mfpu=vfpv4 -mfloat-abi=hard
A9:CC=arm-linux-gnueabihf-gcc

a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a53 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a72:CC=aarch64-linux-gnu-gcc
a72:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a72 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a72:all
a53: all
a57: all
a9: all
x86: all

all: $(FILE_NAME).o
	$(CC) -o $(FILE_NAME) $(FILE_NAME).o $(CC_FLAGS) $(LDFLAGS)

$(FILE_NAME).o: $(FILE_NAME).c
	$(CC) $(CC_FLAGS) -c $(FILE_NAME).c -o $(FILE_NAME).o
clean:
	rm -f *.o

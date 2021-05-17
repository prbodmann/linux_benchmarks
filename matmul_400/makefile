LDFLAGS = -static -lm
TARGET:=arm
CFLAGS = -O3
MATRIX_SIZE = 400

A9:CFLAGS += -mcpu=cortex-a9 -mfpu=vfpv4 -mfloat-abi=hard
A9:CC=arm-linux-gnueabihf-gcc

a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a53 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53: all
a57: all

all:
	$(CC)  $(CFLAGS) $(LDFLAGS) -o matmul_${MATRIX_SIZE} matmul.c -lm


clean:
	rm -rf *.o matmul_* output*

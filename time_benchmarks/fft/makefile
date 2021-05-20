OBJ = main.o fftmisc.o  fourierf.o
FILE = main.c fftmisc.c  fourierf.c
LDFLAGS = -static -lm
SIZE ?= 18
CFLAGS = -O3

x86:CC=gcc
x86:CC_FLAGS = -Ofast -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm


A9:CFLAGS += -mcpu=cortex-a9 -mfpu=vfpv4 -mfloat-abi=hard
A9:CC=arm-linux-gnueabihf-gcc

a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a53 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm
a53: all
a57: all
a9: all
x86: all	

G_OBJ=fft_gen.o fftmisc.o  fourierf.o
G_FILE=fft_gen.c fftmisc.c  fourierf.c

all: ${OBJ} ${G_OBJ}
	$(CC) ${OBJ} -o fft_time ${LDFLAGS}
	$(CC) ${G_OBJ} -o fft_gen ${LDFLAGS}
%.o: %.c
	$(CC) $(CC_FLAGS) -o $@ -c $<
clean:
	rm -rf *.o fft output* gen

OBJ = main.o fftmisc.o  fourierf.o
FILE = main.c fftmisc.c  fourierf.c
LDFLAGS = -static -lm
TARGET:=arm
SIZE ?= 19
CFLAGS = -O3 -DSIZE=${SIZE}


CC.x86=gcc
AS.x86=as
LD.x86=ld
AR.x86=ar

CC.arm64=aarch64-linux-gnu-gcc
AS.arm64=aarch64-linux-gnu-as
LD.arm64=aarch64-linux-gnu-ld
AR.arm64=aarch64-linux-gnu-ar

CFLAGS.arm += -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard
CC.arm=arm-linux-gnueabihf-gcc
AS.arm=arm-linux-gnueabihf-as
LD.arm=arm-linux-gnueabihf-ld
AR.arm=arm-linux-gnueabihf-ar

CFLAGS.armA5 += -mcpu=cortex-a5 -mfpu=vfpv4-d16 -mfloat-abi=hard
CC.armA5=arm-linux-gnueabihf-gcc
AS.armA5=arm-linux-gnueabihf-as
LD.armA5=arm-linux-gnueabihf-ld
AR.armA5=arm-linux-gnueabihf-ar

CC:=${CC.${TARGET}}
AS:=${AS.${TARGET}}
LD:=${LD.${TARGET}}
AR:=${AR.${TARGET}}
CFLAGS += ${CFLAGS.${TARGET}}

G_OBJ=fft_gen.o fftmisc.o  fourierf.o
G_FILE=fft_gen.c fftmisc.c  fourierf.c

all: ${OBJ} ${G_OBJ}
	$(CC) ${OBJ} -o fft ${LDFLAGS}
	$(CC) ${G_OBJ} -o gen ${LDFLAGS}
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	rm -rf *.o fft output*

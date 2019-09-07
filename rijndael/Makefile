LDFLAGS = -static -lm
TARGET:=arm
CFLAGS = -O3


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

all: rijndael_dec rijndael_enc

rijndael_dec: aes.c aes.h aestab.h aesxam.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o rijndael_dec aes.c aesxam.c
rijndael_enc: aes.c aes.h aestab.h aesxam.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o rijndael_enc aes.c aesxam.c
clean:
	rm -rf rijndael_enc rijndael_dec

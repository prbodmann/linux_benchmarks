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

all:susan_s susan_e susan_c

susan_s: susan.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o susan_s susan.c -lm
susan_e: susan.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o susan_e susan.c -lm
susan_c: susan.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o susan_c susan.c -lm
clean:
	rm -rf susan output*

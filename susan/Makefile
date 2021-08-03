LDFLAGS = -static -lm
TARGET:=arm


a9:CC_FLAGS += -mcpu=cortex-a9 -mfpu=vfpv4 -mfloat-abi=hard
a9:CC=arm-linux-gnueabihf-gcc

a5:CC_FLAGS += -mcpu=cortex-a9 -mfpu=vfpv4 -mfloat-abi=hard
a5:CC=arm-linux-gnueabihf-gcc

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
a5:all	

all:susan
susan: susan.c susan_gen.c
	$(CC) $(CC_FLAGS) $(LDFLAGS) -o susan_gen susan_gen.c -lm
	$(CC) $(CC_FLAGS) $(LDFLAGS) -o susan_s susan.c -lm
	cp susan_s susan_e
	cp susan_s susan_c
clean:
	rm -rf susan_* output*

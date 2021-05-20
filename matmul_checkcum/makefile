LDFLAGS = -static -lm
OBJ = matmul.o checksum.o
G_OBJ = mxm_gen.o  checksum.o
FILE = main.c fftmisc.c  fourierf.c
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

all: ${OBJ} ${G_OBJ}
	$(CC)  $(CC_FLAGS) $(LDFLAGS) -o matmul_checksum ${OBJ} -lm
	$(CC) $(CC_FLAGS) $(LDFLAGS)  -o matmul_gen ${G_OBJ} -lm
%.o: %.c
	$(CC) $(CC_FLAGS) -o $@ -c $<
clean:
	rm -rf *.o matmul output* matmul_checksum

LDFLAGS = -static -lm



A9:CFLAGS += -mcpu=cortex-a9 -mfpu=vfpv4 -mfloat-abi=hard
A9:CC=arm-linux-gnueabihf-gcc

a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a53 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53: all
a57: all
a9: all

all:  qsort.o
	$(CC)  $(CC_FLAGS) $(LDFLAGS) -o qsort_time qsort.o 
%.o: %.c
	$(CC) $(CC_FLAGS) -o $@ -c $<

clean:
	rm -rf *.o qsort output*

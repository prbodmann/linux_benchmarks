PRECISION ?= 64

# Suggested flags for thunderxs taken from prace 'Best Practice Guide - ARM64'
a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a53 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53: all
a57: all
all:l1_test
l1_test: l1_test.c
	$(CC) $(CC_FLAGS) -DFP=$(PRECISION) l1_test.c -o l1_test -lm -static

clean:
	rm -f l1_test

# Display help message
help:
	@echo "make options: 'ARCH' and 'PRECISION'"
	@echo "ARCH: 'generic', 'knc', 'thunderx2', 'a57'"
	@echo "PRECISION: '32', '64'"
	@echo "Default: ARCH=generic PRECISION=64"
	@echo ""
	@echo "Compiling for thunderx2 using single precision: make ARCH=thunderx2 PRECISION=32"
	@echo "Compiling for knc using double precision: make ARCH=knc PRECISION=64"

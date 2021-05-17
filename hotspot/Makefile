PRECISION ?= 64

# Suggested flags for thunderxs taken from prace 'Best Practice Guide - ARM64'

a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm



EXE_BASENAME=hotspot_$(PRECISION)

a53: all
a57: all

all: hotspot hotspot_gen

hotspot: hotspot_check.c
	$(CC) $(CC_FLAGS) -DFP=$(PRECISION) hotspot_check.c -o $(EXE_BASENAME) -lm -static
hotspot_gen: hotspot_gen.c
	$(CC) $(CC_FLAGS) -DFP=$(PRECISION) hotspot_gen.c -o $(EXE_BASENAME)_gen -lm -static

clean:
	rm -f hotspot*gen hotspot_$(PRECISION)

# Display help message
help:
	@echo "make options: 'ARCH' and 'PRECISION'"
	@echo "ARCH: 'generic', 'knc', 'thunderx2', 'a57'"
	@echo "PRECISION: '32', '64'"
	@echo "Default: ARCH=generic PRECISION=64"
	@echo ""
	@echo "Compiling for thunderx2 using single precision: make ARCH=thunderx2 PRECISION=32"
	@echo "Compiling for knc using double precision: make ARCH=knc PRECISION=64"

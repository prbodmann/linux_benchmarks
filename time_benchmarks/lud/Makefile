SOURCES_CHECK=lud_check.c lud_omp.c

# General configuration
a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a53 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm
x86: CC=gcc
x86:CC_FLAGS = -Ofast -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

PRECISION = 64                
EXE_BASENAME=lud

a53: all
a57: all
x86: all
all: lud_gen lud_check

lud_gen: lud_gen.c common.c
	$(CC) -DPRECISION=$(PRECISION) common.c lud_omp.c lud_gen.c -o $(EXE_BASENAME)_gen  $(CC_FLAGS) -fopenmp 

lud_check: $(SOURCES_CHECK)
	$(CC) -DPRECISION=$(PRECISION) common.c $(SOURCES_CHECK) -DLOGS -o $(EXE_BASENAME)_time $(CC_FLAGS) -static

clean:
	rm -f lud*gen lud*check lud*err_inj lud*timing

# Display help message
help:
	@echo "make options: 'ARCH' and 'PRECISION'"
	@echo "ARCH: 'generic', 'knc', 'thunderx2'"
	@echo "PRECISION: '32', '64'"
	@echo "Default: ARCH=generic PRECISION=64"
	@echo ""
	@echo "Compiling for thunderx2 using single precision: make ARCH=thunderx2 PRECISION=32"
	@echo "Compiling for knc using double precision: make ARCH=knc PRECISION=64"




SOURCES_CHECK=main_check.c kernel/kernel_cpu.c 
SOURCES_GEN=main.c kernel/kernel_cpu.c
APP_NAME=lavamd
x86: CC=gcc
x86:CC_FLAGS = -Ofast -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a57:CC=aarch64-linux-gnu-gcc
a57:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a57 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a53:CC=aarch64-linux-gnu-gcc
a53:CC_FLAGS = -Ofast -march=armv8-a -mcpu=cortex-a53 -floop-optimize -falign-loops -falign-labels -falign-functions -falign-jumps -fomit-frame-pointer -funroll-loops -lm

a57: lava_generate lavamd 

a53: lava_generate lavamd

x86: lava_generate lavamd

lava_generate: $(SOURCES_GEN)
	$(CC) $(SOURCES_GEN) -lm  -o $(APP_NAME)_gen $(CC_FLAGS) -static

lavamd: $(SOURCES_CHECK)
	$(CC) $(SOURCES_CHECK) -lm -o $(APP_NAME)_time $(LIBRARY) $(CC_FLAGS) -static

clean:
	rm -f input_* output_* $(APP_NAME)_gen $(APP_NAME)_check $(APP_NAME)_timing $(APP_NAME)

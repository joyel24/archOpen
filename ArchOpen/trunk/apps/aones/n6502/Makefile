include ../../../Makefile.conf

ASFLAGS  += -I../../../include -I../../../medios/include -I. -I../
GCCFLAGS += -I../../../include -I../../../medios/include -I. -I../ -O3

BUILD_DIR = ../build

all: do_build ncpu.o

ncpu.o:
	@echo "[$@]"
	@$(GCC) $(GCCFLAGS) -c ncpu.S -o ncpu.o
	@cp ncpu.o $(BUILD_DIR)

include ../../../default.rule
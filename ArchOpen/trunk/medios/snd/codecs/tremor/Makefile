include $(MEDIOS_SRC_PATH)/Makefile.conf

LDFLAGS = $(MEDIOS_SRC_PATH)/lib/codec_start.o $(MEDIOS_SRC_PATH)/lib/codec_stop.o -L$(MEDIOS_SRC_PATH)/lib -lmedios --emit-relocs -Map $(TARGET_CODEC).map 

GCCFLAGS += -O3 -D_ARM_ASSEM_

BUILD_DIR=./
TARGET_CODEC=tremor

all: do_build
	@$(LD) -o $(TARGET_CODEC).bin $(OBJS) $(LDFLAGS)
	@cat codec.info $(TARGET_CODEC).bin > $(TARGET_CODEC).cod
	@rm -f $(TARGET_CODEC).bin
	@mv $(TARGET_CODEC).cod $(MEDIOS_SRC_PATH)/codec

include $(MEDIOS_SRC_PATH)/default.rule
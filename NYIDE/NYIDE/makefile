# Makefile for Andes 


PLAT ?= AG101P
ADDR ?= 16MB
MODE ?= LOAD
FEAT ?= ""
DEBU ?= NO
SIMU ?= NO

OPT_AG101 := n
OPT_16MBADR := y
OPT_LLINIT := n
OPT_REMAP  := n
CROSS_COMPILE ?= nds32le-elf-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
OBJ_PATH := OBJ/
LD_PATH  = ./
INC_PATH = -I. \
	-Isrc \
	-Isrc/Driver \
	-Isrc/Kernel \
	-Isrc/Table \
	-Isrc/Table/IO \
	-Isrc/Table/Midi \
	-Isrc/Table/Voice \
	-Isrc/Table/Midi/Table \
	-Isrc/Table/Vr
DEPS = $(OBJ_PATH)/src/delay.d $(OBJ_PATH)/src/Driver/nx1_adc.d $(OBJ_PATH)/src/Driver/nx1_dac.d $(OBJ_PATH)/src/Driver/nx1_gpio.d $(OBJ_PATH)/src/Driver/nx1_i2c.d $(OBJ_PATH)/src/Driver/nx1_intc.d $(OBJ_PATH)/src/Driver/nx1_ir.d $(OBJ_PATH)/src/Driver/nx1_pwm.d $(OBJ_PATH)/src/Driver/nx1_rtc.d $(OBJ_PATH)/src/Driver/nx1_sdc.d $(OBJ_PATH)/src/Driver/nx1_smu.d $(OBJ_PATH)/src/Driver/nx1_spi.d $(OBJ_PATH)/src/Driver/nx1_timer.d $(OBJ_PATH)/src/Driver/nx1_uart.d $(OBJ_PATH)/src/Driver/nx1_wdt.d $(OBJ_PATH)/src/interrupt.d $(OBJ_PATH)/src/Kernel/adpcm_audioapi.d $(OBJ_PATH)/src/Kernel/adpcm_ch2_audioapi.d $(OBJ_PATH)/src/Kernel/audio_callback.d $(OBJ_PATH)/src/Kernel/audioctl.d $(OBJ_PATH)/src/Kernel/celp_audioapi.d $(OBJ_PATH)/src/Kernel/io_event.d $(OBJ_PATH)/src/Kernel/isr.d $(OBJ_PATH)/src/Kernel/midi_audioapi.d $(OBJ_PATH)/src/Kernel/pcm_audioapi.d $(OBJ_PATH)/src/Kernel/pcm_ch2_audioapi.d $(OBJ_PATH)/src/Kernel/pcm_ch3_audioapi.d $(OBJ_PATH)/src/Kernel/sbc_audioapi.d $(OBJ_PATH)/src/Kernel/sbc_ch2_audioapi.d $(OBJ_PATH)/src/Kernel/vioctl.d $(OBJ_PATH)/src/Kernel/vrctl.d $(OBJ_PATH)/src/key_xip.d $(OBJ_PATH)/src/main.d $(OBJ_PATH)/src/mem.d $(OBJ_PATH)/src/resource.d $(OBJ_PATH)/src/sys.d $(OBJ_PATH)/src/Table/Io/IoTab.d $(OBJ_PATH)/src/Table/Voice/VoiceTab.d $(OBJ_PATH)/src/Table/Vr/VRCmdTab.d $(OBJ_PATH)/src/usr1_xip.d $(OBJ_PATH)/src/init-default.d $(OBJ_PATH)/src/crt0.d 
OBJS = $(OBJ_PATH)/src/delay.o $(OBJ_PATH)/src/Driver/nx1_adc.o $(OBJ_PATH)/src/Driver/nx1_dac.o $(OBJ_PATH)/src/Driver/nx1_gpio.o $(OBJ_PATH)/src/Driver/nx1_i2c.o $(OBJ_PATH)/src/Driver/nx1_intc.o $(OBJ_PATH)/src/Driver/nx1_ir.o $(OBJ_PATH)/src/Driver/nx1_pwm.o $(OBJ_PATH)/src/Driver/nx1_rtc.o $(OBJ_PATH)/src/Driver/nx1_sdc.o $(OBJ_PATH)/src/Driver/nx1_smu.o $(OBJ_PATH)/src/Driver/nx1_spi.o $(OBJ_PATH)/src/Driver/nx1_timer.o $(OBJ_PATH)/src/Driver/nx1_uart.o $(OBJ_PATH)/src/Driver/nx1_wdt.o $(OBJ_PATH)/src/interrupt.o $(OBJ_PATH)/src/Kernel/adpcm_audioapi.o $(OBJ_PATH)/src/Kernel/adpcm_ch2_audioapi.o $(OBJ_PATH)/src/Kernel/audio_callback.o $(OBJ_PATH)/src/Kernel/audioctl.o $(OBJ_PATH)/src/Kernel/celp_audioapi.o $(OBJ_PATH)/src/Kernel/io_event.o $(OBJ_PATH)/src/Kernel/isr.o $(OBJ_PATH)/src/Kernel/midi_audioapi.o $(OBJ_PATH)/src/Kernel/pcm_audioapi.o $(OBJ_PATH)/src/Kernel/pcm_ch2_audioapi.o $(OBJ_PATH)/src/Kernel/pcm_ch3_audioapi.o $(OBJ_PATH)/src/Kernel/sbc_audioapi.o $(OBJ_PATH)/src/Kernel/sbc_ch2_audioapi.o $(OBJ_PATH)/src/Kernel/vioctl.o $(OBJ_PATH)/src/Kernel/vrctl.o $(OBJ_PATH)/src/key_xip.o $(OBJ_PATH)/src/main.o $(OBJ_PATH)/src/mem.o $(OBJ_PATH)/src/resource.o $(OBJ_PATH)/src/sys.o $(OBJ_PATH)/src/Table/Io/IoTab.o $(OBJ_PATH)/src/Table/Voice/VoiceTab.o $(OBJ_PATH)/src/Table/Vr/VRCmdTab.o $(OBJ_PATH)/src/usr1_xip.o $(OBJ_PATH)/src/init-default.o $(OBJ_PATH)/src/crt0.o 
LIBS = src/a0jMsIH.8W5  libDecode.a
LIB_PATH = ./
LDSCRIPT = NYIDE.ld
CFLAGS = $(INC_PATH) -O2 -g3  -c -fmessage-length=0 -ffunction-sections -fdata-sections
LDFLAGS = -O2 -nostartfiles -static -mcmodel=medium -T$(LDSCRIPT) -Wl,-Map -Wl,map.txt -Wl,--gc-sections -mvh $(OBJS) $(LIBS)
TARGET = NX12P44A-001a

all: mkdir $(TARGET).elf

mkdir:
	@-mkdir $(OBJ_PATH)
	@-mkdir -p $(OBJ_PATH)/src
	@-mkdir -p $(OBJ_PATH)/src/Driver
	@-mkdir -p $(OBJ_PATH)/src/Kernel
	@-mkdir -p $(OBJ_PATH)/src/Table
	@-mkdir -p $(OBJ_PATH)/src/Table/IO
	@-mkdir -p $(OBJ_PATH)/src/Table/Midi
	@-mkdir -p $(OBJ_PATH)/src/Table/Voice
	@-mkdir -p $(OBJ_PATH)/src/Table/Midi/Table
	@-mkdir -p $(OBJ_PATH)/src/Table/Vr
	@-mkdir -p $(OBJ_PATH)/ResourceFiles/VR_Module
	@-mkdir -p $(OBJ_PATH)/src/Table/Vr

$(TARGET).elf: $(OBJS)
	@$(CC) $(LDFLAGS) -o $(TARGET).elf
#	@nds32le-elf-size $(TARGET).elf

$(OBJ_PATH)/%.o : %.c
	@echo Building file ... $<
	@$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<
	
$(OBJ_PATH)/%.o : %.C
	@echo Building file ... $<
	@$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<	
	
$(OBJ_PATH)/%.o : %.s
	@echo Building file ... $<
	@$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<	
	
$(OBJ_PATH)/%.o : %.S
	@echo Building file ... $<
	@$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -o $@ $<

-include $(DEPS)
	
.PHONY: clean
clean:
	@echo Cleaning All Files ...
	@rm -rf $(TARGET).bin
	@rm -rf $(TARGET).elf
	@rm -rf $(TARGET).fpga
	@rm -rf $(TARGET).dbg
	@rm -rf *.map *.out
	@rm -rf map.txt
	@rm -rf $(OBJ_PATH)/*.o
	@rm -rf $(OBJ_PATH)/*.d
	@rm -rf $(OBJ_PATH)/src/*.o
	@rm -rf $(OBJ_PATH)/src/*.d
	@rm -rf $(OBJ_PATH)/src/Driver/*.o
	@rm -rf $(OBJ_PATH)/src/Driver/*.d
	@rm -rf $(OBJ_PATH)/src/Kernel/*.o
	@rm -rf $(OBJ_PATH)/src/Kernel/*.d
	@rm -rf $(OBJ_PATH)/src/Table/*.o
	@rm -rf $(OBJ_PATH)/src/Table/*.d
	@rm -rf $(OBJ_PATH)/src/Table/IO/*.o
	@rm -rf $(OBJ_PATH)/src/Table/IO/*.d
	@rm -rf $(OBJ_PATH)/src/Table/Midi/*.o
	@rm -rf $(OBJ_PATH)/src/Table/Midi/*.d
	@rm -rf $(OBJ_PATH)/src/Table/Voice/*.o
	@rm -rf $(OBJ_PATH)/src/Table/Voice/*.d
	@rm -rf $(OBJ_PATH)/src/Table/Midi/Table/*.o
	@rm -rf $(OBJ_PATH)/src/Table/Midi/Table/*.d
	@rm -rf $(OBJ_PATH)/src/Table/Vr/*.o
	@rm -rf $(OBJ_PATH)/src/Table/Vr/*.d
#	@echo Clean Finished
	
.PHONY: size
size:
	@nds32le-elf-size $(TARGET).elf

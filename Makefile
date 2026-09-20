#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
endif

TOPDIR ?= $(CURDIR)

#---------------------------------------------------------------------------------
# App Metadata
#---------------------------------------------------------------------------------
APP_TITLE   := TimeStranger-NX
APP_AUTHOR  := cbreezy210
APP_VERSION := 1.0.0

#---------------------------------------------------------------------------------
# FORCE 64-bit toolchain
#---------------------------------------------------------------------------------
CC := aarch64-none-elf-gcc
CXX := aarch64-none-elf-g++
AS := aarch64-none-elf-gcc
LD := aarch64-none-elf-g++
AR := aarch64-none-elf-ar
NM := aarch64-none-elf-nm
OBJCOPY := aarch64-none-elf-objcopy
OBJDUMP := aarch64-none-elf-objdump
STRIP := aarch64-none-elf-strip

include $(DEVKITPRO)/libnx/switch_rules

TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	.
DATA		:=	data
INCLUDES	:=	include

ARCH	:=	-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE

CFLAGS	:=	-g -Wall -O2 -ffunction-sections \
			$(ARCH) $(DEFINES)

CFLAGS	+=	$(INCLUDE) -D__SWITCH__

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS	:= -lnx
LIBDIRS	:= $(PORTLIBS) $(LIBNX)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES 	:=	$(OFILES_BIN) $(OFILES_SRC)
export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean all

all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).nro $(TARGET).nacp $(TARGET).elf

else

DEPENDS	:=	$(OFILES:.o=.d)

# EXPLICIT RULES TO BAKE IN METADATA
$(OUTPUT).nro: $(OUTPUT).elf $(OUTPUT).nacp
	@echo creating ... $(notdir $@)
	@elf2nro $(OUTPUT).elf $@ --nacp=$(OUTPUT).nacp

$(OUTPUT).nacp:
	@echo creating ... $(notdir $@)
	@nacptool --create "$(APP_TITLE)" "$(APP_AUTHOR)" "$(APP_VERSION)" $@

$(OUTPUT).elf: $(OFILES)
	@echo linking $(notdir $@)
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

%.o: %.cpp
	@echo compiling $(notdir $<)
	@$(CXX) -MMD -MP $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.c
	@echo compiling $(notdir $<)
	@$(CC) -MMD -MP $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.s
	@echo assembling $(notdir $<)
	@$(AS) $(ASFLAGS) -c $< -o $@

%.bin.o	%_bin.h :	%.bin
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

endif
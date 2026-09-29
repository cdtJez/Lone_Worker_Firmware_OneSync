##############################################################################
# Source files
#
# Paths
BUILD_DIR = bin
SOURCE_DIR = src
#
# C sources
SRCS  = main.c delay.c rfm69hw.c spi.c usart.c inputs.c event.c command.c
SRCS += xstrings.c parameters.c flash.c regs.c rtc.c adc.c crc.c messages.c
SRCS += random.c tx_queue.c sequencer.c machine_state.c fxls8964.c i2c.c
SRCS += charger.c program.c
#
# ASM sources
ASMS =
#
# target
TARGET = mdh0203
#
PYTHON ?= python
# Separate shared analysis repository; override on the make command line.
ANALYSIS_TOOLS_DIR ?= C:/pend/firmware-analysis-tools
MAP_REPORT = $(ANALYSIS_TOOLS_DIR)/map_report.py
STATIC_PROFILER = $(ANALYSIS_TOOLS_DIR)/static_profiler.py
COGNITIVE_COMPLEXITY = $(ANALYSIS_TOOLS_DIR)/cognitive_complexity.py
ANALYSIS_DIFF = $(ANALYSIS_TOOLS_DIR)/analysis_diff.py
# GNU make prerequisites need escaped spaces; recipe arguments are quoted below.
empty :=
space := $(empty) $(empty)
analysis_path = $(subst $(space),\$(space),$(1))
# Use one identity for all firmware outputs and analysis reports in this run.
BUILD_DATE := $(shell $(PYTHON) -c "import datetime; print(datetime.datetime.now().strftime('%Y%m%d-%H%M%S'))")
GIT_ID := $(shell $(PYTHON) -c "import subprocess; r=subprocess.run(['git','describe','--always','--dirty','--abbrev=8','--match',''],capture_output=True,text=True); print(r.stdout.strip() if r.returncode == 0 else 'nogit')")
BUILD_NAME := $(TARGET)-$(BUILD_DATE)-$(GIT_ID)

# Debug-friendly optimization; -g3 below includes macro definitions.
OPT = -Og
#-O0 --no optimization/debug
#-Os --size
#-O2 --speed
#-O3 --max speed

#
# Source file location
vpath %.c $(SOURCE_DIR)
#
##############################################################################
# Startup and system
SRCS += syscalls.c
#
ASMS += device/startup_stm32g030k6tx.s
#
##############################################################################
# Compiler executables
#
CC = arm-none-eabi-gcc
CP = arm-none-eabi-objcopy
SZ = arm-none-eabi-size
OD = arm-none-eabi-objdump
#
##############################################################################
# Programmer executable
#
PROG = st-flash
#
##############################################################################
# CPU & MCU type
#
CPU = -mcpu=cortex-m0plus
MCU = $(CPU) -mthumb
#
##############################################################################
# Build number
BUILDNUM = $(shell awk -f buildinc.awk)
#
##############################################################################
# Macros for gcc
#
# Assembler defines
ASM_DEFS =
#
# C defines
C_DEFS = -DSTM32G030xx
#
# Assembler includes
ASM_INCLUDES =
#
# C includes
C_INCLUDES  = -I inc
C_INCLUDES += -Idrivers/CMSIS/Device/ST/STM32G0xx/Include
C_INCLUDES += -Idrivers/CMSIS/Include
#
# Compiler flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -g3 -fno-eliminate-unused-debug-types -Wall -fdata-sections -ffunction-sections -fstack-usage -fcyclomatic-complexity
#
##############################################################################
# LDFLAGS
#
# link script
LDSCRIPT = device/STM32G030K6TX_FLASH.ld
#
# libraries
LIBS = -lc -lm -lnosys
#
# Linker flags
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(BUILD_NAME).map,--cref -Wl,--no-warn-rwx-segments,--gc-sections
#
# Default build includes firmware and analysis reports.
all: analyse

# Firmware-only build, including the fixed ELF used by the debugger.
firmware: $(BUILD_DIR)/$(BUILD_NAME).hex $(BUILD_DIR)/$(BUILD_NAME).bin $(BUILD_DIR)/$(BUILD_NAME).asm debug-elf
# Build standard outputs first, then run the analysis tools on this build.
analyse: firmware $(call analysis_path,$(MAP_REPORT)) $(call analysis_path,$(STATIC_PROFILER)) $(call analysis_path,$(COGNITIVE_COMPLEXITY)) $(call analysis_path,$(ANALYSIS_DIFF))
	$(PYTHON) "$(MAP_REPORT)" "$(BUILD_DIR)/$(BUILD_NAME).map" -o "$(BUILD_DIR)/$(BUILD_NAME)-map-report.txt"
	@echo Map report: $(BUILD_DIR)/$(BUILD_NAME)-map-report.txt
	$(PYTHON) "$(STATIC_PROFILER)" "$(BUILD_DIR)/$(BUILD_NAME).elf" "$(BUILD_DIR)" --json -o "$(BUILD_DIR)/$(BUILD_NAME)-static-profile.json" --graph "$(BUILD_DIR)/$(BUILD_NAME)-stack-vs-complexity.svg"
	@echo Static profile: $(BUILD_DIR)/$(BUILD_NAME)-static-profile.json
	$(PYTHON) "$(COGNITIVE_COMPLEXITY)" $(foreach src,$(SRCS),"$(SOURCE_DIR)/$(src)") --json > "$(BUILD_DIR)/$(BUILD_NAME)-cognitive-complexity.json"
	@echo Cognitive complexity: $(BUILD_DIR)/$(BUILD_NAME)-cognitive-complexity.json
	$(PYTHON) "$(ANALYSIS_DIFF)" "$(BUILD_DIR)/$(BUILD_NAME)"
#
##############################################################################
# build
#
# List of c objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
vpath %.c $(sort $(dir $(SRCS)))
#
# List of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMS:.s=.o)))
vpath %.s $(sort $(dir $(ASMS)))
# Compile the c files
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
#
# Assemble the s files
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(CC) -x assembler-with-cpp -c $(CFLAGS) $< -o $@
#
# Link the object files
$(BUILD_DIR)/$(BUILD_NAME).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
#	@echo Build number: $(BUILDNUM)
#
# Make an Intel hex file
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(CP) -O ihex $< $@
#
# Make a binary file
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(CP) -O binary -S $< $@
#
# Disassemble the binary file
$(BUILD_DIR)/%.asm: $(BUILD_DIR)/%.bin | $(BUILD_DIR)
	$(OD) -z -D -bbinary -marm $< -Mforce-thumb > $@
#
# Create the build directory
$(BUILD_DIR):
	mkdir $@
#
##############################################################################
#
prog: $(BUILD_DIR)/$(BUILD_NAME).bin
	$(PROG) --connect-under-reset write $< 0x8000000
#
##############################################################################
# clean up the build files
clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/*.lst
	rm -f $(BUILD_DIR)/*.su
	rm -f $(BUILD_DIR)/*.cyclo
	rm -f $(BUILD_DIR)/$(TARGET).hex
	rm -f $(BUILD_DIR)/$(TARGET).elf
	rm -f $(BUILD_DIR)/$(TARGET).bin
	rm -f $(BUILD_DIR)/$(TARGET).map
	rm -f $(BUILD_DIR)/$(TARGET).asm
#
##############################################################################
# Debug information
#
print-%  : ; @echo $* = $($*)
#
##############################################################################

.PHONY: all firmware analyse clean prog debug-elf

debug-elf: $(BUILD_DIR)/$(BUILD_NAME).elf
	$(CP) "$<" "$(BUILD_DIR)/$(TARGET).elf"

################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include App/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ElbAltm.bin \

SECONDARY_LIST += \
ElbAltm.lst \

SECONDARY_SIZE += \
ElbAltm.siz \


# All Target
all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: ElbAltm.elf secondary-outputs

# Tool invocations
ElbAltm.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32v\ElbAltm\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"ElbAltm.map" --specs=nano.specs --specs=nosys.specs -o "ElbAltm.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintf
	@	@
ElbAltm.bin: ElbAltm.elf
	@	riscv-none-embed-objcopy -O binary "ElbAltm.elf"  "ElbAltm.bin"
	@	@
ElbAltm.lst: ElbAltm.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "ElbAltm.elf" > "ElbAltm.lst"
	@	@
ElbAltm.siz: ElbAltm.elf
	@	riscv-none-embed-size --format=berkeley "ElbAltm.elf"
	@	@
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) ElbAltm.elf
	-@
pre-build:
	-..\App\rel\nrcomp.bat
	-@
post-build:
	-xcopy /Y \*.bin ..\_bin
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build pre-build post-build

-include ../makefile.targets

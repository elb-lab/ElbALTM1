################################################################################
# MRS Version: 2.2.0
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
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ElbALTM8.bin \

SECONDARY_LIST += \
ElbALTM8.lst \

SECONDARY_SIZE += \
ElbALTM8.siz \


# All Target
all: 
	$(MAKE) --no-print-directory pre-build 
	$(MAKE) --no-print-directory main-build 
	$(MAKE) --no-print-directory post-build 

pre-build: 
	..\App\rel\nrcomp.bat

main-build: ElbALTM8.elf secondary-outputs

post-build: 
	xcopy /Y \*.bin ..\_bin

# Tool invocations
ElbALTM8.elf: $(OBJS) $(USER_OBJS)
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -T "c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage -Wl,-Map,"ElbALTM8.map" --specs=nano.specs --specs=nosys.specs -o "ElbALTM8.elf" $(OBJS)  $(LIBS) -lprintf
ElbALTM8.bin: ElbALTM8.elf
	@	riscv-none-embed-objcopy -O binary "ElbALTM8.elf" "ElbALTM8.bin"
ElbALTM8.lst: ElbALTM8.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "ElbALTM8.elf" > "ElbALTM8.lst"
ElbALTM8.siz: ElbALTM8.elf
	riscv-none-embed-size --format=berkeley "ElbALTM8.elf"

# Other Targets
clean:
	-$(RM) $(OBJS) $(EXPANDS) $(CALLGRAPH_DOT) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(S_UPPER_DEPS)$(ASM_DEPS)$(ASM_UPPER_DEPS)$(C_DEPS) ElbALTM8.elf

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
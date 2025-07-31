################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Debug" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Core" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\User" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Peripheral\inc" -I../App/inc -I../App/rel -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@


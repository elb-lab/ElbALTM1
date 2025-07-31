################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/core_riscv.c 

OBJS += \
./Core/core_riscv.o 

C_DEPS += \
./Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o: ../Core/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Debug" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Core" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\User" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Peripheral\inc" -I../App/inc -I../App/rel -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@


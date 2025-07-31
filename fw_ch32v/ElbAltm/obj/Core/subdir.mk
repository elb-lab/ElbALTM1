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
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32v\ElbAltm\Debug" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32v\ElbAltm\Core" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32v\ElbAltm\User" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32v\ElbAltm\Peripheral\inc" -I../App/rel -I../App/inc -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@


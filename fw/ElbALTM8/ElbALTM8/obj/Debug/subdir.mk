################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

C_DEPS += \
./Debug/debug.d 

OBJS += \
./Debug/debug.o 


EXPANDS += \
./Debug/debug.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/Debug" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/Core" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/User" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/Peripheral/inc" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/App/inc" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/App/rel" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


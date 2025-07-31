################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/src/atcmd.c \
../App/src/irq.c \
../App/src/led.c \
../App/src/ms5611.c \
../App/src/scp.c \
../App/src/tmr.c \
../App/src/usr.c 

OBJS += \
./App/src/atcmd.o \
./App/src/irq.o \
./App/src/led.o \
./App/src/ms5611.o \
./App/src/scp.o \
./App/src/tmr.o \
./App/src/usr.o 

C_DEPS += \
./App/src/atcmd.d \
./App/src/irq.d \
./App/src/led.d \
./App/src/ms5611.d \
./App/src/scp.d \
./App/src/tmr.d \
./App/src/usr.d 


# Each subdirectory must supply rules for building sources it contributes
App/src/%.o: ../App/src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Debug" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Core" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\User" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Peripheral\inc" -I../App/inc -I../App/rel -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@


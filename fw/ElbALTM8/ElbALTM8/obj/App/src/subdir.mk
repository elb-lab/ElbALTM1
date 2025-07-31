################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/src/irq.c \
../App/src/led.c \
../App/src/ms5611.c \
../App/src/tmr.c \
../App/src/usr.c 

C_DEPS += \
./App/src/irq.d \
./App/src/led.d \
./App/src/ms5611.d \
./App/src/tmr.d \
./App/src/usr.d 

OBJS += \
./App/src/irq.o \
./App/src/led.o \
./App/src/ms5611.o \
./App/src/tmr.o \
./App/src/usr.o 


EXPANDS += \
./App/src/irq.c.234r.expand \
./App/src/led.c.234r.expand \
./App/src/ms5611.c.234r.expand \
./App/src/tmr.c.234r.expand \
./App/src/usr.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
App/src/%.o: ../App/src/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/Debug" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/Core" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/User" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/Peripheral/inc" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/App/inc" -I"c:/dirpers/Progetti x MCU/ElbALTM/fw/ElbALTM8/ElbALTM8/App/rel" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


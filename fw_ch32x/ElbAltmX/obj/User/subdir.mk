################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32x035_it.c \
../User/ch32x035_usbfs_device.c \
../User/main.c \
../User/system_ch32x035.c \
../User/usb_desc.c 

OBJS += \
./User/ch32x035_it.o \
./User/ch32x035_usbfs_device.o \
./User/main.o \
./User/system_ch32x035.o \
./User/usb_desc.o 

C_DEPS += \
./User/ch32x035_it.d \
./User/ch32x035_usbfs_device.d \
./User/main.d \
./User/system_ch32x035.d \
./User/usb_desc.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Debug" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Core" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\User" -I"C:\dirpers\Progetti x MCU\ElbALTM\fw_ch32x\ElbAltmX\Peripheral\inc" -I../App/inc -I../App/rel -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@


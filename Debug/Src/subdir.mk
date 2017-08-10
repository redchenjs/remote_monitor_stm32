################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/crc.c \
../Src/dac.c \
../Src/dcmi.c \
../Src/eth.c \
../Src/fmc.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f7xx.c \
../Src/tim.c \
../Src/usart.c \
../Src/usb_otg.c 

OBJS += \
./Src/crc.o \
./Src/dac.o \
./Src/dcmi.o \
./Src/eth.o \
./Src/fmc.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f7xx.o \
./Src/tim.o \
./Src/usart.o \
./Src/usb_otg.o 

C_DEPS += \
./Src/crc.d \
./Src/dac.d \
./Src/dcmi.d \
./Src/eth.d \
./Src/fmc.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f7xx.d \
./Src/tim.d \
./Src/usart.d \
./Src/usb_otg.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F767xx -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STemWin" -I/home/redchenjs/workspace/stm32/STM32Cube/Repository/STM32Cube_FW_F7_V1.7.0/Middlewares/ST/STemWin/inc -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



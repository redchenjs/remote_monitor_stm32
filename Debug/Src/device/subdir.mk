################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/device/delay.c \
../Src/device/gpio.c \
../Src/device/nvic.c \
../Src/device/sys.c \
../Src/device/tim.c \
../Src/device/usart.c 

OBJS += \
./Src/device/delay.o \
./Src/device/gpio.o \
./Src/device/nvic.o \
./Src/device/sys.o \
./Src/device/tim.o \
./Src/device/usart.o 

C_DEPS += \
./Src/device/delay.d \
./Src/device/gpio.d \
./Src/device/nvic.d \
./Src/device/sys.d \
./Src/device/tim.d \
./Src/device/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/device/%.o: ../Src/device/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F767xx -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STemWin" -I/home/redchenjs/workspace/stm32/STM32Cube/Repository/STM32Cube_FW_F7_V1.7.0/Middlewares/ST/STemWin/inc -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



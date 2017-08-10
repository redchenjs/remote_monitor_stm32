################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/Fonts/font12.c \
../Inc/Fonts/font16.c \
../Inc/Fonts/font20.c \
../Inc/Fonts/font24.c \
../Inc/Fonts/font8.c 

OBJS += \
./Inc/Fonts/font12.o \
./Inc/Fonts/font16.o \
./Inc/Fonts/font20.o \
./Inc/Fonts/font24.o \
./Inc/Fonts/font8.o 

C_DEPS += \
./Inc/Fonts/font12.d \
./Inc/Fonts/font16.d \
./Inc/Fonts/font20.d \
./Inc/Fonts/font24.d \
./Inc/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/Fonts/%.o: ../Inc/Fonts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F767xx -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STemWin" -I/home/redchenjs/workspace/stm32/STM32Cube/Repository/STM32Cube_FW_F7_V1.7.0/Middlewares/ST/STemWin/inc -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



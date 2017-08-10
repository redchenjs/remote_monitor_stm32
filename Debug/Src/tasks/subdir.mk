################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/tasks/task_data.c \
../Src/tasks/task_lcd.c 

OBJS += \
./Src/tasks/task_data.o \
./Src/tasks/task_lcd.o 

C_DEPS += \
./Src/tasks/task_data.d \
./Src/tasks/task_lcd.d 


# Each subdirectory must supply rules for building sources it contributes
Src/tasks/%.o: ../Src/tasks/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F767xx -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STemWin" -I/home/redchenjs/workspace/stm32/STM32Cube/Repository/STM32Cube_FW_F7_V1.7.0/Middlewares/ST/STemWin/inc -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



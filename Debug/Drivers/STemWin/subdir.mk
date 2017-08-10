################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STemWin/GUIConf.c \
../Drivers/STemWin/GUIDRV_NT35510.c \
../Drivers/STemWin/GUI_X.c \
../Drivers/STemWin/LCDConf.c 

OBJS += \
./Drivers/STemWin/GUIConf.o \
./Drivers/STemWin/GUIDRV_NT35510.o \
./Drivers/STemWin/GUI_X.o \
./Drivers/STemWin/LCDConf.o 

C_DEPS += \
./Drivers/STemWin/GUIConf.d \
./Drivers/STemWin/GUIDRV_NT35510.d \
./Drivers/STemWin/GUI_X.d \
./Drivers/STemWin/LCDConf.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STemWin/%.o: ../Drivers/STemWin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F767xx -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STemWin" -I/home/redchenjs/workspace/stm32/STM32Cube/Repository/STM32Cube_FW_F7_V1.7.0/Middlewares/ST/STemWin/inc -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Drivers/CMSIS/Include" -I"/home/redchenjs/workspace/stm32/f767_remote_monitor/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



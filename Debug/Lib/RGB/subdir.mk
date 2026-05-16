################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/RGB/RGB.c 

C_DEPS += \
./Lib/RGB/RGB.d 

OBJS += \
./Lib/RGB/RGB.o 


# Each subdirectory must supply rules for building sources it contributes
Lib/RGB/%.o Lib/RGB/%.su Lib/RGB/%.cyclo: ../Lib/RGB/%.c Lib/RGB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/Watch/Lib/IR_rec" -I"D:/STM/Watch/Task" -I"D:/STM/Watch/u8g2" -I"D:/STM/Watch/Lib/Buzzer" -I"D:/STM/Watch/Lib/DHT11" -I"D:/STM/Watch/Lib/RGB" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib-2f-RGB

clean-Lib-2f-RGB:
	-$(RM) ./Lib/RGB/RGB.cyclo ./Lib/RGB/RGB.d ./Lib/RGB/RGB.o ./Lib/RGB/RGB.su

.PHONY: clean-Lib-2f-RGB


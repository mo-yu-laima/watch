################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/Buzzer/Buzzer.c 

C_DEPS += \
./Lib/Buzzer/Buzzer.d 

OBJS += \
./Lib/Buzzer/Buzzer.o 


# Each subdirectory must supply rules for building sources it contributes
Lib/Buzzer/%.o Lib/Buzzer/%.su Lib/Buzzer/%.cyclo: ../Lib/Buzzer/%.c Lib/Buzzer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/Watch/Lib/IR_rec" -I"D:/STM/Watch/Task" -I"D:/STM/Watch/u8g2" -I"D:/STM/Watch/Lib/Buzzer" -I"D:/STM/Watch/Lib/DHT11" -I"D:/STM/Watch/Lib/RGB" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib-2f-Buzzer

clean-Lib-2f-Buzzer:
	-$(RM) ./Lib/Buzzer/Buzzer.cyclo ./Lib/Buzzer/Buzzer.d ./Lib/Buzzer/Buzzer.o ./Lib/Buzzer/Buzzer.su

.PHONY: clean-Lib-2f-Buzzer


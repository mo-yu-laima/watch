################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Task/MyData.c \
../Task/ShowCalendar.c \
../Task/ShowClockTask.c \
../Task/ShowDHT11Task.c \
../Task/ShowRGBTask.c \
../Task/ShowTimerTask.c \
../Task/ShowmeuTask.c 

C_DEPS += \
./Task/MyData.d \
./Task/ShowCalendar.d \
./Task/ShowClockTask.d \
./Task/ShowDHT11Task.d \
./Task/ShowRGBTask.d \
./Task/ShowTimerTask.d \
./Task/ShowmeuTask.d 

OBJS += \
./Task/MyData.o \
./Task/ShowCalendar.o \
./Task/ShowClockTask.o \
./Task/ShowDHT11Task.o \
./Task/ShowRGBTask.o \
./Task/ShowTimerTask.o \
./Task/ShowmeuTask.o 


# Each subdirectory must supply rules for building sources it contributes
Task/%.o Task/%.su Task/%.cyclo: ../Task/%.c Task/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/Watch/Lib/IR_rec" -I"D:/STM/Watch/Task" -I"D:/STM/Watch/u8g2" -I"D:/STM/Watch/Lib/Buzzer" -I"D:/STM/Watch/Lib/DHT11" -I"D:/STM/Watch/Lib/RGB" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Task

clean-Task:
	-$(RM) ./Task/MyData.cyclo ./Task/MyData.d ./Task/MyData.o ./Task/MyData.su ./Task/ShowCalendar.cyclo ./Task/ShowCalendar.d ./Task/ShowCalendar.o ./Task/ShowCalendar.su ./Task/ShowClockTask.cyclo ./Task/ShowClockTask.d ./Task/ShowClockTask.o ./Task/ShowClockTask.su ./Task/ShowDHT11Task.cyclo ./Task/ShowDHT11Task.d ./Task/ShowDHT11Task.o ./Task/ShowDHT11Task.su ./Task/ShowRGBTask.cyclo ./Task/ShowRGBTask.d ./Task/ShowRGBTask.o ./Task/ShowRGBTask.su ./Task/ShowTimerTask.cyclo ./Task/ShowTimerTask.d ./Task/ShowTimerTask.o ./Task/ShowTimerTask.su ./Task/ShowmeuTask.cyclo ./Task/ShowmeuTask.d ./Task/ShowmeuTask.o ./Task/ShowmeuTask.su

.PHONY: clean-Task


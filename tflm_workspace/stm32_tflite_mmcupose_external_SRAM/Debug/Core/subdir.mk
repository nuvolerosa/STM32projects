################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/lstm_small.cpp \
../Core/main.cpp \
../Core/sine_model.cpp \
../Core/srnn_small.cpp 

C_SRCS += \
../Core/debug_log.c \
../Core/lcd.c \
../Core/stm32f7xx_hal_msp.c \
../Core/stm32f7xx_it.c \
../Core/syscalls.c \
../Core/sysmem.c \
../Core/system_stm32f7xx.c 

C_DEPS += \
./Core/debug_log.d \
./Core/lcd.d \
./Core/stm32f7xx_hal_msp.d \
./Core/stm32f7xx_it.d \
./Core/syscalls.d \
./Core/sysmem.d \
./Core/system_stm32f7xx.d 

OBJS += \
./Core/debug_log.o \
./Core/lcd.o \
./Core/lstm_small.o \
./Core/main.o \
./Core/sine_model.o \
./Core/srnn_small.o \
./Core/stm32f7xx_hal_msp.o \
./Core/stm32f7xx_it.o \
./Core/syscalls.o \
./Core/sysmem.o \
./Core/system_stm32f7xx.o 

CPP_DEPS += \
./Core/lstm_small.d \
./Core/main.d \
./Core/sine_model.d \
./Core/srnn_small.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o Core/%.su Core/%.cyclo: ../Core/%.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Core -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../tensorflow -I../Drivers/BSP/Components -I../Drivers/BSP/STM32746G-Discovery -I../Utilities/Fonts -I../third_party/flatbuffers/include -I../third_party/gemmlowp -I../third_party/ruy -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/%.o Core/%.su Core/%.cyclo: ../Core/%.cpp Core/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Core -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../tensorflow -I../Drivers/BSP/Components -I../Drivers/BSP/STM32746G-Discovery -I../Utilities/Fonts -I../third_party/flatbuffers/include -I../third_party/gemmlowp -I../third_party/ruy -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core

clean-Core:
	-$(RM) ./Core/debug_log.cyclo ./Core/debug_log.d ./Core/debug_log.o ./Core/debug_log.su ./Core/lcd.cyclo ./Core/lcd.d ./Core/lcd.o ./Core/lcd.su ./Core/lstm_small.cyclo ./Core/lstm_small.d ./Core/lstm_small.o ./Core/lstm_small.su ./Core/main.cyclo ./Core/main.d ./Core/main.o ./Core/main.su ./Core/sine_model.cyclo ./Core/sine_model.d ./Core/sine_model.o ./Core/sine_model.su ./Core/srnn_small.cyclo ./Core/srnn_small.d ./Core/srnn_small.o ./Core/srnn_small.su ./Core/stm32f7xx_hal_msp.cyclo ./Core/stm32f7xx_hal_msp.d ./Core/stm32f7xx_hal_msp.o ./Core/stm32f7xx_hal_msp.su ./Core/stm32f7xx_it.cyclo ./Core/stm32f7xx_it.d ./Core/stm32f7xx_it.o ./Core/stm32f7xx_it.su ./Core/syscalls.cyclo ./Core/syscalls.d ./Core/syscalls.o ./Core/syscalls.su ./Core/sysmem.cyclo ./Core/sysmem.d ./Core/sysmem.o ./Core/sysmem.su ./Core/system_stm32f7xx.cyclo ./Core/system_stm32f7xx.d ./Core/system_stm32f7xx.o ./Core/system_stm32f7xx.su

.PHONY: clean-Core


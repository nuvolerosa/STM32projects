################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tensorflow/tensorflow/lite/c/common.c 

C_DEPS += \
./tensorflow/tensorflow/lite/c/common.d 

OBJS += \
./tensorflow/tensorflow/lite/c/common.o 


# Each subdirectory must supply rules for building sources it contributes
tensorflow/tensorflow/lite/c/%.o tensorflow/tensorflow/lite/c/%.su tensorflow/tensorflow/lite/c/%.cyclo: ../tensorflow/tensorflow/lite/c/%.c tensorflow/tensorflow/lite/c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Core -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../tensorflow -I../Drivers/BSP/Components -I../Drivers/BSP/STM32746G-Discovery -I../Utilities/Fonts -I../third_party/flatbuffers/include -I../third_party/gemmlowp -I../third_party/ruy -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tensorflow-2f-tensorflow-2f-lite-2f-c

clean-tensorflow-2f-tensorflow-2f-lite-2f-c:
	-$(RM) ./tensorflow/tensorflow/lite/c/common.cyclo ./tensorflow/tensorflow/lite/c/common.d ./tensorflow/tensorflow/lite/c/common.o ./tensorflow/tensorflow/lite/c/common.su

.PHONY: clean-tensorflow-2f-tensorflow-2f-lite-2f-c


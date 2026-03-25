################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../tensorflow/tensorflow/lite/kernels/internal/quantization_util.cc 

CC_DEPS += \
./tensorflow/tensorflow/lite/kernels/internal/quantization_util.d 

OBJS += \
./tensorflow/tensorflow/lite/kernels/internal/quantization_util.o 


# Each subdirectory must supply rules for building sources it contributes
tensorflow/tensorflow/lite/kernels/internal/%.o tensorflow/tensorflow/lite/kernels/internal/%.su tensorflow/tensorflow/lite/kernels/internal/%.cyclo: ../tensorflow/tensorflow/lite/kernels/internal/%.cc tensorflow/tensorflow/lite/kernels/internal/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Core -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../tensorflow -I../Drivers/BSP/Components -I../Drivers/BSP/STM32746G-Discovery -I../Utilities/Fonts -I../third_party/flatbuffers/include -I../third_party/gemmlowp -I../third_party/ruy -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tensorflow-2f-tensorflow-2f-lite-2f-kernels-2f-internal

clean-tensorflow-2f-tensorflow-2f-lite-2f-kernels-2f-internal:
	-$(RM) ./tensorflow/tensorflow/lite/kernels/internal/quantization_util.cyclo ./tensorflow/tensorflow/lite/kernels/internal/quantization_util.d ./tensorflow/tensorflow/lite/kernels/internal/quantization_util.o ./tensorflow/tensorflow/lite/kernels/internal/quantization_util.su

.PHONY: clean-tensorflow-2f-tensorflow-2f-lite-2f-kernels-2f-internal


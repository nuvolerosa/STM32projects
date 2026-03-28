################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../tensorflow/tensorflow/lite/micro/testing/test_utils.cc 

CC_DEPS += \
./tensorflow/tensorflow/lite/micro/testing/test_utils.d 

OBJS += \
./tensorflow/tensorflow/lite/micro/testing/test_utils.o 


# Each subdirectory must supply rules for building sources it contributes
tensorflow/tensorflow/lite/micro/testing/%.o tensorflow/tensorflow/lite/micro/testing/%.su tensorflow/tensorflow/lite/micro/testing/%.cyclo: ../tensorflow/tensorflow/lite/micro/testing/%.cc tensorflow/tensorflow/lite/micro/testing/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Core -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../tensorflow -I../Drivers/BSP/Components -I../Drivers/BSP/STM32746G-Discovery -I../Utilities/Fonts -I../third_party/flatbuffers/include -I../third_party/gemmlowp -I../third_party/ruy -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tensorflow-2f-tensorflow-2f-lite-2f-micro-2f-testing

clean-tensorflow-2f-tensorflow-2f-lite-2f-micro-2f-testing:
	-$(RM) ./tensorflow/tensorflow/lite/micro/testing/test_utils.cyclo ./tensorflow/tensorflow/lite/micro/testing/test_utils.d ./tensorflow/tensorflow/lite/micro/testing/test_utils.o ./tensorflow/tensorflow/lite/micro/testing/test_utils.su

.PHONY: clean-tensorflow-2f-tensorflow-2f-lite-2f-micro-2f-testing


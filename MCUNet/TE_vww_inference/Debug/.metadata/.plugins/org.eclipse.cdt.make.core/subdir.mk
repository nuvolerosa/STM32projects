################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.make.core/specs.c 

CPP_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.make.core/specs.cpp 

C_DEPS += \
./.metadata/.plugins/org.eclipse.cdt.make.core/specs.d 

OBJS += \
./.metadata/.plugins/org.eclipse.cdt.make.core/specs.o 

CPP_DEPS += \
./.metadata/.plugins/org.eclipse.cdt.make.core/specs.d 


# Each subdirectory must supply rules for building sources it contributes
.metadata/.plugins/org.eclipse.cdt.make.core/specs.o: ../.metadata/.plugins/org.eclipse.cdt.make.core/specs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"/home/nicole/STM32CubeIDE/M1/TE_vww_tutorial/Src/TinyEngine/include" -I"/home/nicole/STM32CubeIDE/M1/TE_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I"/home/nicole/STM32CubeIDE/M1/TE_vww_tutorial/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF".metadata/.plugins/org.eclipse.cdt.make.core/specs.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
.metadata/.plugins/org.eclipse.cdt.make.core/specs.o: ../.metadata/.plugins/org.eclipse.cdt.make.core/specs.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"/home/nicole/STM32CubeIDE/M1/TE_vww_tutorial/Src/TinyEngine/include" -I"/home/nicole/STM32CubeIDE/M1/TE_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"/home/nicole/STM32CubeIDE/M1/TE_vww_tutorial/Src/TinyEngine/codegen/Include" -I"/home/nicole/STM32CubeIDE/M1/TE_vww_tutorial/Src/TinyEngine/include" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF".metadata/.plugins/org.eclipse.cdt.make.core/specs.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"


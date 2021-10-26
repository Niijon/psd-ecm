################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f407vgtx.s 

OBJS += \
./Core/Startup/startup_stm32f407vgtx.o 

S_DEPS += \
./Core/Startup/startup_stm32f407vgtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"N:/PUTSolarDynamics/Klara/psd-ecm/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"N:/PUTSolarDynamics/Klara/psd-ecm/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"N:/PUTSolarDynamics/Klara/psd-ecm/Drivers/CMSIS/Include" -I"N:/PUTSolarDynamics/Klara/psd-ecm/USB_DEVICE/Target" -I"N:/PUTSolarDynamics/Klara/psd-ecm/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"N:/PUTSolarDynamics/Klara/psd-ecm/Core/Inc" -I"N:/PUTSolarDynamics/Klara/psd-ecm/Drivers/STM32F4xx_HAL_Driver/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"


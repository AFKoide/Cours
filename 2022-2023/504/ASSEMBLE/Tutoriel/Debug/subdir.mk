################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../main.s \
../startup_stm32f303xc.s 

OBJS += \
./main.o \
./startup_stm32f303xc.o 

S_DEPS += \
./main.d \
./startup_stm32f303xc.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.s subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean--2e-

clean--2e-:
	-$(RM) ./main.d ./main.o ./startup_stm32f303xc.d ./startup_stm32f303xc.o

.PHONY: clean--2e-


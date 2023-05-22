################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/joystick.c \
../Src/joystick_HAL.c \
../Src/main.c \
../Src/ptu_actuation.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/joystick.o \
./Src/joystick_HAL.o \
./Src/main.o \
./Src/ptu_actuation.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/joystick.d \
./Src/joystick_HAL.d \
./Src/main.d \
./Src/ptu_actuation.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303VCTx -DSTM32 -DSTM32F3 -DSTM32F3DISCOVERY -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/joystick.d ./Src/joystick.o ./Src/joystick.su ./Src/joystick_HAL.d ./Src/joystick_HAL.o ./Src/joystick_HAL.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/ptu_actuation.d ./Src/ptu_actuation.o ./Src/ptu_actuation.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src


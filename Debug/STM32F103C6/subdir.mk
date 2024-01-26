################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F103C6/STM32_F103C6_EXTI-Driver.c \
../STM32F103C6/STM32_F103C6_GPIO-Driver.c \
../STM32F103C6/STM32_F103C6_I2C-Driver.c \
../STM32F103C6/STM32_F103C6_RCC-Driver.c \
../STM32F103C6/STM32_F103C6_SPI-Driver.c \
../STM32F103C6/STM32_F103C6_USART-Driver.c 

OBJS += \
./STM32F103C6/STM32_F103C6_EXTI-Driver.o \
./STM32F103C6/STM32_F103C6_GPIO-Driver.o \
./STM32F103C6/STM32_F103C6_I2C-Driver.o \
./STM32F103C6/STM32_F103C6_RCC-Driver.o \
./STM32F103C6/STM32_F103C6_SPI-Driver.o \
./STM32F103C6/STM32_F103C6_USART-Driver.o 

C_DEPS += \
./STM32F103C6/STM32_F103C6_EXTI-Driver.d \
./STM32F103C6/STM32_F103C6_GPIO-Driver.d \
./STM32F103C6/STM32_F103C6_I2C-Driver.d \
./STM32F103C6/STM32_F103C6_RCC-Driver.d \
./STM32F103C6/STM32_F103C6_SPI-Driver.d \
./STM32F103C6/STM32_F103C6_USART-Driver.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F103C6/STM32_F103C6_EXTI-Driver.o: ../STM32F103C6/STM32_F103C6_EXTI-Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6/STM32_F103C6_EXTI-Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6/STM32_F103C6_GPIO-Driver.o: ../STM32F103C6/STM32_F103C6_GPIO-Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6/STM32_F103C6_GPIO-Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6/STM32_F103C6_I2C-Driver.o: ../STM32F103C6/STM32_F103C6_I2C-Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6/STM32_F103C6_I2C-Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6/STM32_F103C6_RCC-Driver.o: ../STM32F103C6/STM32_F103C6_RCC-Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6/STM32_F103C6_RCC-Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6/STM32_F103C6_SPI-Driver.o: ../STM32F103C6/STM32_F103C6_SPI-Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6/STM32_F103C6_SPI-Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32F103C6/STM32_F103C6_USART-Driver.o: ../STM32F103C6/STM32_F103C6_USART-Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32F103C6/STM32_F103C6_USART-Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


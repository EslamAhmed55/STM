################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HALL/I2C_Slave_EEPROM.c \
../HALL/keypad.c \
../HALL/lcd.c 

OBJS += \
./HALL/I2C_Slave_EEPROM.o \
./HALL/keypad.o \
./HALL/lcd.o 

C_DEPS += \
./HALL/I2C_Slave_EEPROM.d \
./HALL/keypad.d \
./HALL/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
HALL/I2C_Slave_EEPROM.o: ../HALL/I2C_Slave_EEPROM.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HALL/I2C_Slave_EEPROM.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HALL/keypad.o: ../HALL/keypad.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HALL/keypad.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
HALL/lcd.o: ../HALL/lcd.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"D:/courses/course/embedded system/keroles shenouda/STM32F103C6_Drivers/RemoteSystemsTempFiles" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/STM32F103C6/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -I"D:/courses/course/embedded system/keroles shenouda/Drivers/STM32F103C6/Drivers/HALL/includes" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HALL/lcd.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


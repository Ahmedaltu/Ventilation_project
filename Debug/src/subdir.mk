################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DigitalIoPin.cpp \
../src/Fan.cpp \
../src/I2C.cpp \
../src/IntegerEdit.cpp \
../src/LiquidCrystal.cpp \
../src/LpcUart.cpp \
../src/MQTT.cpp \
../src/MenuItem.cpp \
../src/Modbus_Drive.cpp \
../src/SimpleMenu.cpp \
../src/StringEdit.cpp \
../src/Vantilation_project.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc15xx.cpp \
../src/retarget_uart.cpp \
../src/socket_mqtt.cpp 

C_SRCS += \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/DigitalIoPin.o \
./src/Fan.o \
./src/I2C.o \
./src/IntegerEdit.o \
./src/LiquidCrystal.o \
./src/LpcUart.o \
./src/MQTT.o \
./src/MenuItem.o \
./src/Modbus_Drive.o \
./src/SimpleMenu.o \
./src/StringEdit.o \
./src/Vantilation_project.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc15xx.o \
./src/crp.o \
./src/retarget_uart.o \
./src/socket_mqtt.o \
./src/sysinit.o 

CPP_DEPS += \
./src/DigitalIoPin.d \
./src/Fan.d \
./src/I2C.d \
./src/IntegerEdit.d \
./src/LiquidCrystal.d \
./src/LpcUart.d \
./src/MQTT.d \
./src/MenuItem.d \
./src/Modbus_Drive.d \
./src/SimpleMenu.d \
./src/StringEdit.d \
./src/Vantilation_project.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc15xx.d \
./src/retarget_uart.d \
./src/socket_mqtt.d 

C_DEPS += \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=c++11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



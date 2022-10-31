################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/paho.mqtt.embedded-c/MQTTClient-C/test/test1.c 

OBJS += \
./src/paho.mqtt.embedded-c/MQTTClient-C/test/test1.o 

C_DEPS += \
./src/paho.mqtt.embedded-c/MQTTClient-C/test/test1.d 


# Each subdirectory must supply rules for building sources it contributes
src/paho.mqtt.embedded-c/MQTTClient-C/test/%.o: ../src/paho.mqtt.embedded-c/MQTTClient-C/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



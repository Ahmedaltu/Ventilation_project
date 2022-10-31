################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectClient.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectServer.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTDeserializePublish.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTFormat.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTPacket.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSerializePublish.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeClient.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeServer.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeClient.c \
../src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeServer.c 

OBJS += \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectClient.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectServer.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTDeserializePublish.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTFormat.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTPacket.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSerializePublish.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeClient.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeServer.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeClient.o \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeServer.o 

C_DEPS += \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectClient.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTConnectServer.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTDeserializePublish.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTFormat.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTPacket.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSerializePublish.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeClient.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTSubscribeServer.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeClient.d \
./src/paho.mqtt.embedded-c/MQTTPacket/src/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
src/paho.mqtt.embedded-c/MQTTPacket/src/%.o: ../src/paho.mqtt.embedded-c/MQTTPacket/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\ahmed\Documents\MCUXpressoIDE_11.2.1_4149\workspace\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



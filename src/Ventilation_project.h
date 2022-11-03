/*
 * Ventilation_project.h
 *
 *  Created on: 21 Oct 2022
 *      Author: ahmed
 */

#ifndef VENTILATION_PROJECT_H_
#define VENTILATION_PROJECT_H_


#include"board.h"
#include "DigitalIoPin.h"
#include "retarget_uart.h"
#include <atomic>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
//#include "Sleep.h"
#include "LpcUart.h"
#include "ritimer_15xx.h"
#include "LiquidCrystal.h"
#include "systick.h"
#include "I2C.h"
//#include "ITMwarpper.h"
#include "./modbus/ModbusMaster.h"
#include "./modbus/ModbusRegister.h"
#include "SimpleMenu.h"
#include "IntegerEdit.h"
#include "StringEdit.h"
#include "Modbus_Drive.h"
#include "EventQueue.h"

#include "json.hpp"

#include "./mqtt/esp8266_socket.h"
#include "paho.mqtt/MQTTClient-C/MQTTClient.h"
#include "jsmn.h"

#include <cr_section_macros.h>
#include <atomic>
#include "MQTT.h"

#define SSID	    "SmartIotMQTT"
#define PASSWORD    "SmartIot"
#define BROKER_IP   (char *)"192.168.1.254"
#define BROKER_PORT  1883

#define MQTT_TOPIC_RECEIVE_SET   (const char *)"controller/settings"
#define MQTT_TOPIC_SEND_STATUS      (const char *)"G07/controller/status"
#define MQTT_TOPIC_SEND_SETTING     (const char *)"controller/settings"

#define DEBOUNCE             20


int16_t getPressure();
void mqtt_message_handler(MessageData* data);
static std::string mqtt_message("");
static bool mqtt_message_arrived(false);

static volatile uint32_t systicks(0);
static volatile uint32_t last_pressed(0);
static SimpleMenu menu;
static StringEdit *mode;
#endif /* VENTILATION_PROJECT_H_ */

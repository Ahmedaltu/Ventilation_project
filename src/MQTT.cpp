/*
 * MQTT.cpp
 *
 *  Created on: 24 Oct 2022
 *      Author: ahmed
 */




#include "MQTT.h"
#include <string>


MQTT::MQTT(void (*message_handler_)(MessageData* data)) :
	client(), network(), message_handler(message_handler_)
{
	connect_data = MQTTPacket_connectData_initializer;
}


int MQTT::connect(const char* ssid, const char* pass, char* ip, int port)
{
	// Connect to network and initialize MQTT
	NetworkInit(&network, ssid, pass);
	MQTTClientInit(&client, &network, MQTT_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

	// Connect to broker
	status = NetworkConnect(&network, ip, port);
	connect_data.MQTTVersion = MQTT_VERSION;
	connect_data.clientID.cstring = (char *)CLIENT_ID;

	status = MQTTConnect(&client, &connect_data);

	return status;
}

void MQTT::disconnect()
{
	NetworkDisconnect(&network);
}

int MQTT::subscribe(const char* topic)
{
	status = MQTTSubscribe(&client, topic, QOS2, message_handler);
	return status;
}

int MQTT::publish(const char* topic, char* data, size_t data_size)
{
	MQTTMessage message;

	message.qos = QOS1;
	message.retained = 0;
	message.payload = data;
	message.payloadlen = data_size;

	status = MQTTPublish(&client, topic, &message);
	return status;
}

int MQTT::publish(const char* topic, std::string data, size_t data_size)
{
	return MQTT::publish(topic, &data[0], data_size);
}

int MQTT::yield(int duration)
{
	status = MQTTYield(&client, duration);
	return status;
}

int MQTT::get_status() const
{
	return status;
}

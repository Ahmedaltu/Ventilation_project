/*
 * Fan.h
 *
 *  Created on: 19 Oct 2022
 *      Author: ahmed
 */

#ifndef FAN_H_
#define FAN_H_
#include "./modbus/ModbusMaster.h"
#include "./modbus/ModbusRegister.h"



void Sleep(int ms);

class Fan {
public:
	Fan();
	virtual ~Fan();

	bool setFrequency(int freq);
	uint16_t getFrequency();
private:
	ModbusMaster node;
};

#endif /* FAN_H_ */

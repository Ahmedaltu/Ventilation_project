/*
 * Fan.cpp
 *
 *  Created on: 19 Oct 2022
 *      Author: ahmed
 */



#include "Fan.h"
#include <cstring>
#include <cstdio>

#include "./modbus/ModbusMaster.h"
#include "./modbus/ModbusRegister.h"

#include "board.h"
#include "chip.h"


Fan::Fan() {
	ModbusMaster node(1);
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	ModbusRegister AO1(&node, 0);
	ModbusRegister DI1(&node, 4, false);

	//ControlWord.write(0x0406); // prepare for starting
	//AO1.write(0x0406);
    //Sleep(1000); // give converter some time to set up

	//ControlWord = 0x047F; // set drive to start mode
    //DI1.write(0x0406);
	//Sleep(1000);
}

Fan::~Fan() {
	// TODO Auto-generated destructor stub
}

bool Fan::setFrequency(int freq)
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	node.writeSingleRegister(1, freq); // set motor frequency

	printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = node.readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node.ku8MBSuccess) {
			if(node.getResponseBuffer(0) & 0x0100) atSetpoint = true;
		}
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}

uint16_t Fan::getFrequency() {
	//ModbusRegister OutputFrequency{ &node, 102 };
	ModbusRegister OutputFrequency(&node, 4, false);
	uint16_t result = OutputFrequency.read();
	return result;
}

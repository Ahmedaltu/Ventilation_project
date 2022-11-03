/*
 * Modbus_Drive.h
 *
 *  Created on: 2 Nov 2022
 *      Author: ahmed
 */

#ifndef MODBUS_DRIVE_H_
#define MODBUS_DRIVE_H_

#include "./modbus/ModbusMaster.h"
#include "./modbus/ModbusRegister.h"

#define fan_ID               1
#define HMP60_ID             241
#define GMP252_ID            240

#define TRANSMISSION_RATE    9600

#define write_fan_r_ADD          0
#define read_fan_r_ADD           4

#define read_temp_r_ADD          0x0101
#define read_rh_r_ADD            0x0100

#define read_co2_r_ADD           0x0101

#define Error_rh_r_ADD           0x0200

#define TIMEOUT_LIMIT        20
#define SUCCESS_MASK         0x0100
#define BREAK_TIME           1000


class Modbus_Drive {
public:
	Modbus_Drive();
	void set_frequency(const uint16_t& freq);
	int get_frequency();
	int get_temp();
	int get_rh();
	int get_co2();


private:
	ModbusMaster fan_node; // Create modbus object that connects to slave id 1
	ModbusMaster HMP_node; //rh&temp
	ModbusMaster GMP_node; //co2



	ModbusRegister write_fan_r;
	ModbusRegister read_fan_r;
	ModbusRegister read_temp_r;
	ModbusRegister read_rh_r;
    ModbusRegister read_co2_r;
    ModbusRegister Error_rh_r;

};




#endif /* MODBUS_DRIVE_H_ */

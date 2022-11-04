/*
 * Modbus_Drive.cpp
 *
 *  Created on: 2 Nov 2022
 *      Author: ahmed
 */




#include "chip.h"
#include "delay.h"
#include "Modbus_Drive.h"

/**
 * @brief ABB Drive class encapsulates frequency control
 * Modbus interface is utilized
 */

Modbus_Drive::Modbus_Drive():
	fan_node(fan_ID), // Create modbus object that connects to slave id
    HMP_node(HMP60_ID), //rh&temp
    GMP_node(GMP252_ID), //co2

    write_fan_r(&fan_node, write_fan_r_ADD),
    read_fan_r(&fan_node, read_fan_r_ADD, false),
	read_temp_r(&HMP_node, read_temp_r_ADD, true),
	read_rh_r(&HMP_node, read_rh_r_ADD, true),
	read_co2_r(&GMP_node, read_co2_r_ADD, true),
	Error_rh_r(&HMP_node, Error_rh_r_ADD, true)
{
	fan_node.begin(TRANSMISSION_RATE);
	HMP_node.begin(TRANSMISSION_RATE);
	GMP_node.begin(TRANSMISSION_RATE);



}


void Modbus_Drive::set_frequency(const uint16_t& freq)
{
	int frequency = freq;

	write_fan_r.write(frequency*10);


}

//Just for checking fan plus
int Modbus_Drive::get_frequency()
{
	return read_fan_r.read();
}
int Modbus_Drive::get_temp(){
	/*
	bool ready =false;
	int timeout = 0;
	while(!ready && timeout < TIMEOUT_LIMIT ){
		delay_systick(500);
		if (read_fan_r.read()) ready =true;

	}
	*/

	return read_temp_r.read()/10;
	delay_systick(500);


}



int Modbus_Drive::get_rh(){

	bool ready =false;
	int timeout = 0;
	/*while(!Error_rh_r.read() && !ready){
		delay_systick(500);

		if (read_rh_r.read()) ready =true;

	}*/


    return read_rh_r.read()/10;
    delay_systick(500);


}
int Modbus_Drive::get_co2(){
	/*bool ready =false;
	int timeout = 0;
	while(!ready && timeout < TIMEOUT_LIMIT ){

		if (read_co2_r.read()) ready =true;
	}*/


   return (read_co2_r.read()/10)*100;
   delay_systick(500);
}







/*
 * LCD_task1.cpp
 *
 *  Created on: 7 Oct 2022
 *      Author: ahmed
 */


#if 1
#include "Ventilation_project.h"
static volatile std::atomic_int counter;

#ifdef __cplusplus


extern "C" {
	void SysTick_Handler(void)
	{
		systicks++;
		if(counter > 0) counter--;
	}
	uint32_t get_ticks(void){
		return systicks;
	}

	void PIN_INT0_IRQHandler(void)
	{
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));

		// Button debounce
		if (millis() - last_pressed < DEBOUNCE) return;
		last_pressed = millis();

		menu.event(MenuItem::up);
	}

	void PIN_INT1_IRQHandler(void)
	{
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));

		// Button debounce
		if (millis() - last_pressed < DEBOUNCE) return;
		last_pressed = millis();

		if(menu.getIndex()==0 ) menu.event(MenuItem::ok);

		if (mode->getValue() == 0 &&  menu.getIndex()==1 ) menu.event(MenuItem::ok);

		if (mode->getValue() == 1 && menu.getIndex()==2) menu.event(MenuItem::ok);
	}

	void PIN_INT2_IRQHandler(void)
	{
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));

		// Button debounce
		if (millis() - last_pressed < DEBOUNCE) return;
		last_pressed = millis();

		menu.event(MenuItem::down);
	}
}
#endif

void Sleep(int ms)
{
	counter = ms;
		while(counter > 0) {
			__WFI(); // this put cpu in sleep mood until and interput
		}
}





std::string sample_json(int sample_number, int speed, int setpoint,int pressure, const char* mode, const char* error, int co2, int rh, int temp);
//void handle_mqtt_input(current_mode, current_speed, current_pressure));
int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable


	SystemCoreClockUpdate();

#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	//char buffer[100];
	// this call initializes debug uart for stdout redirection
	retarget_init();
	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

    Chip_RIT_Init(LPC_RITIMER);

	DigitalIoPin *rs = new DigitalIoPin(0, 29, DigitalIoPin::output);
	DigitalIoPin *en = new DigitalIoPin(0, 9, DigitalIoPin::output);
	DigitalIoPin *d4 = new DigitalIoPin(0, 10, DigitalIoPin::output);
	DigitalIoPin *d5 = new DigitalIoPin(0, 16, DigitalIoPin::output);
	DigitalIoPin *d6 = new DigitalIoPin(1, 3, DigitalIoPin::output);
	DigitalIoPin *d7 = new DigitalIoPin(0, 0, DigitalIoPin::output);
	LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

	DigitalIoPin sw1(1, 8, DigitalIoPin::pullup, true);
	DigitalIoPin sw2(0, 5, DigitalIoPin::pullup, true);
	DigitalIoPin sw3(0, 6, DigitalIoPin::pullup, true);
	DigitalIoPin sw4(0, 7, DigitalIoPin::pullup, true);

	DigitalIoPin::init_gpio_interrupts();
	sw1.enable_interrupt(0);
	sw2.enable_interrupt(1);
	sw3.enable_interrupt(2);

    ModbusMaster node(1); // Create modbus object that connects to slave id 1
    ModbusMaster node1(241); //rh&temp
    ModbusMaster node2(240); //co2

	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here
	node1.begin(9600);
	node2.begin(9600);

	ModbusRegister AO1(&node, 0);
	ModbusRegister DI1(&node, 4, false);

	ModbusRegister DI11(&node1, 0x0101, true); //temp
	ModbusRegister DI12(&node1, 0x0100, true); //rh

	ModbusRegister DI21(&node2, 0x0101, true); //c02


	//SimpleMenu menu;


	std::string modes[2] = { "Manual", "Automatic" };
	mode= new StringEdit(&lcd, std::string("Mode"), modes, 2);
	IntegerEdit *target_Speed= new IntegerEdit(&lcd, std::string("Target Speed"), 100, 0, 1);
	IntegerEdit *target_pressure= new IntegerEdit(&lcd, std::string("Target Pressure"), 120, 0, 1);
	IntegerEdit *speed= new IntegerEdit(&lcd, std::string("Speed"), 100, 0, 1);
	IntegerEdit *pressure= new IntegerEdit(&lcd, std::string("Pressure"), 120, 0, 1);
	IntegerEdit *co2= new IntegerEdit(&lcd, std::string("CO2"), 120, 0, 1);
	IntegerEdit *rh= new IntegerEdit(&lcd, std::string("RH"), 120, 0, 1);
	IntegerEdit *temp= new IntegerEdit(&lcd, std::string("Temp"), 120, 0, 1);


	menu.addItem(new MenuItem(mode));
	menu.addItem(new MenuItem(target_Speed));
	menu.addItem(new MenuItem(target_pressure));
	menu.addItem(new MenuItem(speed));
	menu.addItem(new MenuItem(pressure));
	menu.addItem(new MenuItem(co2));
	menu.addItem(new MenuItem(rh));
	menu.addItem(new MenuItem(temp));


	mode->setValue(0);
	target_Speed->setValue(0);
	target_pressure->setValue(0);
	speed->setValue(0);
	pressure->setValue(0);
	co2->setValue(0);
	rh->setValue(0);
	temp->setValue(0);

	menu.event(MenuItem::show);

	MQTT mqtt(mqtt_message_handler);
	mqtt.connect(SSID, PASSWORD, BROKER_IP, BROKER_PORT);
	mqtt.subscribe(MQTT_TOPIC_RECEIVE_SET);
	int mqtt_status = 0;
	int nr=0;
	AO1.write(0);
	char* statu[] = { "false", "true"};


	jsmn_parser p;
	jsmntok_t tokens[128]; // a number >= total number of tokens
	while(1){

		pressure->setValue(getPressure());
		Sleep(200);
		int tem=DI11.read()/10;
		temp->setValue(tem);
		Sleep(200);
		int humi=DI12.read()/10;
		rh->setValue(humi);
		Sleep(200);
		int co=DI21.read();
		co2->setValue(co*10);
		//Sleep(200);




		int current_speed= speed->getValue();
		int current_pressure= getPressure();





		if (StringEdit::string_changed == true || IntegerEdit::integer_changed == true ) { //|| values_updates= true
			if ( menu.getIndex()==1 ){
				if(current_speed !=target_Speed->getValue()){

					AO1.write(target_Speed->getValue()*10);
					Sleep(200);
					speed->setValue(target_Speed->getValue());

				}
				pressure->setValue(getPressure());

			}

			else if(menu.getIndex()==2){
				if(target_pressure->getValue() > current_pressure){
					while(getPressure() < target_pressure->getValue()){
						current_speed+=1;
						AO1.write(current_speed*10);
						Sleep(200);


					}
					pressure->setValue(getPressure());
					speed->setValue(current_speed);



				}
				else if(target_pressure->getValue() < current_pressure){
					while(getPressure() > target_pressure->getValue()){
						current_speed-=1;
						AO1.write(current_speed*10);
						Sleep(200);

					}
					pressure->setValue(getPressure());
					speed->setValue(current_speed*10);
				}

			}


			StringEdit::string_changed = false;
			IntegerEdit::integer_changed = false;
		}




		if(mqtt_message_arrived){
			mqtt_message_arrived=false;
			printf((mqtt_message + "\r\n").c_str());
            const char *mqtt_message_ = mqtt_message.c_str();
			jsmn_parse(&p, mqtt_message_, 50, tokens, 256);

			jsmntok_t key = tokens[2];
			unsigned int length = key.end - key.start;
			char set_mode[length+1];
			memcpy(set_mode, &mqtt_message_[key.start], length);
			set_mode[length] = '\0';
			printf("Key: %s\n", set_mode);

			jsmntok_t key_ = tokens[4];
			unsigned int length_ = key_.end - key_.start;
			char set_point[length_ + 1];
			memcpy(set_point, &mqtt_message_[key_.start], length_);
			set_point[length] = '\0';
			printf("Key: %s\n", set_point);
			int speed_updated = std::stoi(set_point);

			if(strncmp("false", set_mode, 5)== 0){
				AO1.write(speed_updated*10);
				Sleep(200);
				target_Speed->setValue(speed_updated*10);
				speed->setValue(speed_updated*10);

			}else{
				target_pressure->setValue(speed_updated);
				if(target_pressure->getValue() > current_pressure){
					while(getPressure() < target_pressure->getValue()){
						current_speed+=1;
						AO1.write(current_speed*10);
						Sleep(200);


					}
					pressure->setValue(getPressure());
					speed->setValue(current_speed*10);



				}
				else if(target_pressure->getValue() < current_pressure){
					while(getPressure() > target_pressure->getValue()){
						current_speed-=1;
						AO1.write(current_speed*10);
						Sleep(200);

					}
					pressure->setValue(getPressure());
					speed->setValue(current_speed*10);
				}

			}

		}

		std::string sample = sample_json(nr,speed->getValue(), 10,pressure->getValue(), statu[mode->getValue()], statu[mqtt_status], co, humi, tem);
	    mqtt_status = mqtt.publish(MQTT_TOPIC_SEND_STATUS,  sample, sample.length());
		Sleep(2000); //every 2 seconds
		nr++;
		menu.event(MenuItem::show);
		mqtt.yield(2000);

	}
	return 0;
}
void mqtt_message_handler(MessageData* data)
{
	mqtt_message_arrived = true;
	mqtt_message = "";

	// Parse message from payload
	char payload_parsed[READ_BUF_LENGTH];
	snprintf(
			payload_parsed,
			data->message->payloadlen + 1,
			"%.*s",
			data->message->payloadlen,
			(char *)data->message->payload
	);
	mqtt_message = payload_parsed;
}
void delay_systick(const int ticks)
{
	int delay = ticks;
	while (delay > 0) {
		__WFI();
	}
}

int16_t getPressure(){
	uint8_t tx[1];
	uint8_t rx[3];
	tx[0]=0xF1;
	I2C_config cfgg ;
	I2C i2c(cfgg);
	int16_t pressure;
	if (i2c.transaction(0x40,tx,1,rx,3)) {

		// Combine reading bytes
		uint16_t raw = (rx[0] << 8) | rx[1];
		// uint containing two's complement -> int16
		if (raw & 0x8000) {
			raw = -~(raw - 1);
		}
		pressure = raw;
		// Scale value
		pressure /= 240;
		// Correct considering altitude
		pressure *= 0.95;
	}

	return pressure;
}

std::string sample_json(int sample_number, int speed, int setpoint,int pressure, const char* mode, const char* error, int co2, int rh, int temp)
{

	nlohmann::json sample;
	sample["nr"] = sample_number;
	sample["speed"] = speed;
	sample["setpoint"] = setpoint;
	sample["pressure"] = pressure;
	sample["auto"] = mode;
	sample["error"] = error;
	sample["co2"] = co2;
	sample["rh"] = rh;
	sample["temp"] = temp;


	return sample.dump();
}


uint32_t millis() {
	return systicks;
}
#endif



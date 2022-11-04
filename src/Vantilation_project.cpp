/*
 * LCD_task1.cpp
 *
 *  Created on: 7 Oct 2022
 *      Author: ahmed
 */


#if 1
#include "Ventilation_project.h"


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

		//Just focusing at the mode and target pressure and temperature with conditions.
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
    //LCD configuration
	DigitalIoPin *rs = new DigitalIoPin(0, 29, DigitalIoPin::output);
	DigitalIoPin *en = new DigitalIoPin(0, 9, DigitalIoPin::output);
	DigitalIoPin *d4 = new DigitalIoPin(0, 10, DigitalIoPin::output);
	DigitalIoPin *d5 = new DigitalIoPin(0, 16, DigitalIoPin::output);
	DigitalIoPin *d6 = new DigitalIoPin(1, 3, DigitalIoPin::output);
	DigitalIoPin *d7 = new DigitalIoPin(0, 0, DigitalIoPin::output);
	LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

	//Buttons configuration
	DigitalIoPin sw1(1, 8, DigitalIoPin::pullup, true);
	DigitalIoPin sw2(0, 5, DigitalIoPin::pullup, true);
	DigitalIoPin sw3(0, 6, DigitalIoPin::pullup, true);
	DigitalIoPin sw4(0, 7, DigitalIoPin::pullup, true);

	//Enabling interrupt
	DigitalIoPin::init_gpio_interrupts();
	sw1.enable_interrupt(0);
	sw2.enable_interrupt(1);
	sw3.enable_interrupt(2);



    //Items that will be displayed in the LCD
	std::string modes[2] = { "Manual", "Automatic" };
	mode= new StringEdit(&lcd, std::string("Mode"), modes, 2);
	IntegerEdit *target_Speed= new IntegerEdit(&lcd, std::string("Target Speed"), 100, 0, 1);
	IntegerEdit *target_pressure= new IntegerEdit(&lcd, std::string("Target Pressure"), 120, 0, 1);
	IntegerEdit *speed= new IntegerEdit(&lcd, std::string("Speed"), 100, 0, 1);
	IntegerEdit *pressure= new IntegerEdit(&lcd, std::string("Pressure"), 120, 0, 1);
	IntegerEdit *co2= new IntegerEdit(&lcd, std::string("CO2"), 10000, 0, 1);
	IntegerEdit *rh= new IntegerEdit(&lcd, std::string("RH"), 100, 0, 1);
	IntegerEdit *temp= new IntegerEdit(&lcd, std::string("Temp"), 60, -40, 1);


	menu.addItem(new MenuItem(mode));
	menu.addItem(new MenuItem(target_Speed));
	menu.addItem(new MenuItem(target_pressure));
	menu.addItem(new MenuItem(speed));
	menu.addItem(new MenuItem(pressure));
	menu.addItem(new MenuItem(co2));
	menu.addItem(new MenuItem(rh));
	menu.addItem(new MenuItem(temp));

	//Setting initial items values with 0
	mode->setValue(0);
	target_Speed->setValue(0);
	target_pressure->setValue(0);
	speed->setValue(0);
	pressure->setValue(0);
	co2->setValue(0);
	rh->setValue(0);
	temp->setValue(0);

	menu.event(MenuItem::show);

	//MQTT and WIFI connection initialization and configuration
	MQTT mqtt(mqtt_message_handler);
	mqtt.connect(SSID, PASSWORD, BROKER_IP, BROKER_PORT);
	mqtt.subscribe(MQTT_TOPIC_RECEIVE_SET);

	int mqtt_status = 0;
	int nr=0;
	char* statu[] = { "false", "true"};


	jsmn_parser p;
	jsmntok_t tokens[256]; // a number >= total number of tokens

	//Initializing Modbus connection.
	Modbus_Drive mBus;
	mBus.set_frequency(0);


	while(1){

		//Getting sensors values.
		pressure->setValue(getPressure());

		int tem=mBus.get_temp();      //-40 ... +60°C
		temp->setValue(tem);

		int humi=mBus.get_rh();       // 0 ... 100 %RH
		rh->setValue(humi);

		int co=mBus.get_co2();        // 0 … 10 000 ppm CO2, working range -40 … +60 °C
		co2->setValue(co);



		int current_speed= speed->getValue();
		int current_pressure= getPressure();


		if (StringEdit::string_changed == true || IntegerEdit::integer_changed == true ) {
			if ( menu.getIndex()==1 ){
				if(current_speed !=target_Speed->getValue()){
					mBus.set_frequency(target_Speed->getValue()*10);
					Sleep(200);
					speed->setValue(target_Speed->getValue());
					pressure->setValue(getPressure());
				}
			}

			menu.event(MenuItem::show);
			StringEdit::string_changed = false;
			IntegerEdit::integer_changed = false;
		}


		if(mqtt_message_arrived){                  //Handling the coming setting from web UI
			jsmn_init(&p);
			mqtt_message_arrived=false;

			//printf((mqtt_message + "\r\n").c_str());
            const char *mqtt_message_ = mqtt_message.c_str();
			jsmn_parse(&p, mqtt_message_, 100, tokens, 256);

			//Getting the mode value
			jsmntok_t key = tokens[2];
			unsigned int length = key.end - key.start;
			char set_mode[length+1];
			memcpy(set_mode, &mqtt_message_[key.start], length);
			set_mode[length] = '\0';
			//printf("Key: %s\n", set_mode);

			//Getting the setpoint value
			jsmntok_t key_ = tokens[4];
			unsigned int length_ = key_.end - key_.start;
			char set_point[length_ + 1];
			memcpy(set_point, &mqtt_message_[key_.start], length_);
			set_point[length_] = '\0';
			//printf("Key: %s\n", set_point);
			int speed_updated = std::stoi(set_point);

			if(strncmp("false", set_mode, 5)== 0){
				mode->setValue(0);
				target_Speed->setValue(speed_updated);
				mBus.set_frequency(speed_updated*10);
				Sleep(200);

				speed->setValue(speed_updated);


			}else if (strncmp("true", set_mode, 4) == 0){
				mode->setValue(1);
				target_pressure->setValue(speed_updated);

			}
			menu.event(MenuItem::show);
			memset(tokens,0,256);

		}
		if(mode->getValue() == 1){
			int tolerance = 2;

			if(abs(target_pressure->getValue() - current_pressure) > tolerance){
				double diff = (target_pressure->getValue() - current_pressure) / (double)120;
				signed int diff_ = (signed int)(diff * 50);

				current_speed += diff_;
				/* limiting the values within the range */
				if(current_speed > 100) current_speed = 100;
				if(current_speed < 0) current_speed = 0;
			}
			mBus.set_frequency(current_speed*10);
			Sleep(200);
			speed->setValue(current_speed);

		}



		std::string sample = sample_json(nr,speed->getValue(), 10,pressure->getValue(), statu[mode->getValue()], statu[mqtt_status], co, humi, tem);
	    mqtt_status = mqtt.publish(MQTT_TOPIC_SEND_STATUS,  sample, sample.length());
		Sleep(2000); //every 2 seconds
		nr++;
		menu.event(MenuItem::show);


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



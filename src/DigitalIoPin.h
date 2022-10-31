/*
 * DigitalIoPin.h
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

class DigitalIoPin {
public:
	enum pinMode {
		output,
		input,
		pullup,
		pulldown
	};
	DigitalIoPin(int port, int pin, pinMode mode, bool invert = false);
	virtual ~DigitalIoPin();
	virtual bool read();
	void write(bool value);

	void enable_interrupt(const int& pin_irq_index);
	static void init_gpio_interrupts();

private:
	int port;
	int pin;
	bool inv;
	//const bool is_input;
	//const bool is_invert;
	//void init_as_input(const bool& pullup);
	//void init_as_output();

};

#endif /* DIGITALIOPIN_H_ */

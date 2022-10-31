/*
 * DigitalIoPin.cpp
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */

#include "DigitalIoPin.h"
#include "chip.h"


DigitalIoPin::DigitalIoPin(int port_, int pin_, pinMode mode, bool invert) : port(port_), pin(pin_), inv(invert) {
	if(mode == output) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_MODE_INACT | IOCON_DIGMODE_EN);
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}
	else {
		uint32_t pm = IOCON_DIGMODE_EN;

		if(invert) pm |= IOCON_INV_EN;

		if(mode == pullup) {
			pm |= IOCON_MODE_PULLUP;
		}
		else if(mode == pulldown) {
			pm |= IOCON_MODE_PULLDOWN;
		}

		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, pm);
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
}

DigitalIoPin::~DigitalIoPin() {
	// TODO Auto-generated destructor stub
}

bool DigitalIoPin::read() {
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

void DigitalIoPin::write(bool value) {
	return Chip_GPIO_SetPinState(LPC_GPIO, port, pin, inv ^ value);
}

void DigitalIoPin::enable_interrupt(const int& pin_irq_index)
{

	Chip_INMUX_PinIntSel(pin_irq_index, port, pin);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(pin_irq_index));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(pin_irq_index));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(pin_irq_index));

	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ((IRQn_Type)(PIN_INT0_IRQn + pin_irq_index));
	NVIC_EnableIRQ((IRQn_Type)(PIN_INT0_IRQn + pin_irq_index));
}

/**
 * @brief Initialize pin interrupt
 * Static, effects entire system
 */
void DigitalIoPin::init_gpio_interrupts()
{
	/* Initialize PININT driver */
	Chip_PININT_Init(LPC_GPIO_PIN_INT);
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);
	Chip_SYSCTL_PeriphReset(RESET_PININT);
}

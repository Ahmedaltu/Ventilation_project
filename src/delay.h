/*
 * delay.h
 *
 *  Created on: 2 Nov 2022
 *      Author: ahmed
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>


void delay_systick(const int ticks);

#ifdef __cplusplus
extern "C" {
#endif
uint32_t millis();
#ifdef __cplusplus
}
#endif



#endif /* DELAY_H_ */

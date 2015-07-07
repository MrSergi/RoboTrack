/*
 * delay.h
 *
 *  Created on: 11.11.2012
 *      Author: Александр Горбачев
 */

#ifndef __DELAY_H_
#define __DELAY_H_

#include "conf.h"

void simple_delay(__IO uint32_t nCount);
void simple_delay_ms(__IO uint32_t nCount);
void simple_delay_us(__IO uint32_t nCount);

#endif /* DELAY_H_ */

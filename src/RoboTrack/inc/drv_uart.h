/*
 * uart1.h
 *
 *  Created on: 02.06.2013
 *      Author: Александр Горбачев
 */

#ifndef __UART1_H_
#define __UART1_H_

#include "conf.h"

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE    128
#endif

// typedef void (* uartReceiveCallbackPtr)(uint16_t data);     // used by uart driver to return data to app


void uartInit(uint32_t speed);
void uartChangeBaud(uint32_t speed);
bool uartTransmitEmpty(void);
void uartWrite(uint8_t ch);
void USART_OutChar(char ch);
void USART_OutString(char *s);

extern char   rx_buffer[UART_BUFFER_SIZE];
extern uint16_t  rx_wr_index, //индекс хвоста буфера (куда писать данные)
                 rx_rd_index, //индекс начала буфера (откуда читать данные)
                 rx_counter; //количество данных в буфере
extern uint8_t rx_buffer_overflow;


#endif /* __UART1_H_ */

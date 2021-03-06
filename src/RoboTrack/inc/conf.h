/*
 * ���� ������������ ��� ������� ADC
 */


#ifndef __CONF_H_
#define __CONF_H_

// for roundf()
#define __USE_C99_MATH

//#define USE_SENSOR_IRQ
#define LCD_CONF
#define OLED_I2C_128x64

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "board-01.h"
#include "stm32f10x_conf.h"
#include "core_cm3.h"
#include "drv_spi.h"
#include "drv_system.h"
#include "drv_i2c.h"
#include "delay.h"
#include "LCD.h"

/* ���-�� ������ ��� �������� � 1 �����������. ������������ � ������ delay.c */
#define SIMPLE_DELAY_CYCLES	7000

/* UART */
typedef void (* uartReceiveCallbackPtr)(uint16_t data);     // used by uart driver to return data to app

#define UART_BUFFER_SIZE    1

#include "drv_uart.h"

/* ������ Watchdog */
//#define LSI_TIM_MEASURE		// Use TIM5 to measure LSI frequency - �� �������������� � LD �����������

#include "wdg.h"

typedef struct health_t {
	uint32_t healtFlag;
	uint32_t reserv;
} health_t;

extern health_t baro_unit_health;

typedef struct baro_sensor_t {
	float alt;      		/* ������ [��] */
	float airSpeed;			/* ��������� �������� [�/�] */
	float static_pressure; 	/* ����������� �������� [����] */
	float dynamic_pressure; /* ������������ �������� [���] */
	float temper; 			/* ����������� [�C] */
} baro_sensor_t;

extern baro_sensor_t baro;

extern u16 timercmd;

void msgSend(void);
void msgInit(void);
void msgParse(CanRxMsg* RxMessage);


/* ������ Watchdog */
//#define LSI_TIM_MEASURE		// Use TIM5 to measure LSI frequency - �� �������������� � LD �����������

#endif

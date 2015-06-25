/*

	Плата Вычислителя Воздушных Сигналов ADC 110-17-01-01

CPU			STM32F103RET6
FLASH		512KB
RAM			64KB

LED_STS		PB0		Высокий уровень - светодиод включен
LED_ERR		PB1

Внешний CAN интерфейс
CAN_RX		PA11
CAN_TX		PA12

Внешний COM порт	
UART1_RX	PA10
UART1_TX	PA9
*/

#ifndef __BOARD_H_
#define __BOARD_H_

/*
--- Светодиоды - Высокий уровень - светодиод включен
LED_GREEN	PB0
LED_RED		PB1
*/
// Green
#define LED_STS_GPIO   		GPIOC
#define LED_STS_PIN    		GPIO_Pin_14
// Red
#define LED_ERR_GPIO   		GPIOC
#define LED_ERR_PIN    		GPIO_Pin_13
// Blue
#define LED_NAV_GPIO   		GPIOB
#define LED_NAV_PIN    		GPIO_Pin_14

/* Последовательные порты */
#define	UART1_TX_GPIO		GPIOA
#define	UART1_TX_PIN		GPIO_Pin_9

#define	UART1_RX_GPIO		GPIOA
#define	UART1_RX_PIN		GPIO_Pin_10

/* Полезные макросы */
#define digitalHi(p, i)     { p->BSRR = i; }
#define digitalLo(p, i)     { p->BRR = i; }
#define digitalToggle(p, i) { p->ODR ^= i; }

#define LED_TOGGLE			digitalToggle(LED_GPIO, LED_PIN);
#define LED_OFF             digitalHi(LED_GPIO, LED_PIN);
#define LED_ON              digitalLo(LED_GPIO, LED_PIN);


// Servo 1
#define SERVO1_PIN    		GPIO_Pin_9

// Servo 2
#define SERVO2_PIN    		GPIO_Pin_8

// Servo 3
#define SERVO3_PIN    		GPIO_Pin_7

// Servo 4
#define SERVO4_PIN    		GPIO_Pin_6

#define SERVO_GPIO   		GPIOB


//#define SERVO1_ON	digitalHi(LED_GPIO, LED_PIN)
//#define SERVO2_ON	digitalHi(LED_GPIO, LED_PIN)
//#define SERVO3_ON	digitalHi(LED_GPIO, LED_PIN)
//#define SERVO4_ON	digitalHi(LED_GPIO, LED_PIN)
//
//#define SERVO1_OFF	digitalLo(LED_GPIO, LED_PIN)
//#define SERVO2_OFF	digitalLo(LED_GPIO, LED_PIN)
//#define SERVO3_OFF	digitalLo(LED_GPIO, LED_PIN)
//#define SERVO4_OFF	digitalLo(LED_GPIO, LED_PIN)

#define ROBOT_STOP 			digitalLo(SERVO_GPIO, SERVO1_PIN);digitalLo(SERVO_GPIO, SERVO2_PIN);digitalLo(SERVO_GPIO, SERVO3_PIN);digitalLo(SERVO_GPIO, SERVO4_PIN);
#define ROBOT_FORWARD 		digitalLo(SERVO_GPIO, SERVO1_PIN);digitalHi(SERVO_GPIO, SERVO2_PIN);digitalLo(SERVO_GPIO, SERVO3_PIN);digitalHi(SERVO_GPIO, SERVO4_PIN);
#define ROBOT_BACKWARD 		digitalHi(SERVO_GPIO, SERVO1_PIN);digitalLo(SERVO_GPIO, SERVO2_PIN);digitalHi(SERVO_GPIO, SERVO3_PIN);digitalLo(SERVO_GPIO, SERVO4_PIN);
#define ROBOT_LEFT 			digitalLo(SERVO_GPIO, SERVO1_PIN);digitalHi(SERVO_GPIO, SERVO2_PIN);digitalHi(SERVO_GPIO, SERVO3_PIN);digitalLo(SERVO_GPIO, SERVO4_PIN);
#define ROBOT_RIGHT 		digitalHi(SERVO_GPIO, SERVO1_PIN);digitalLo(SERVO_GPIO, SERVO2_PIN);digitalLo(SERVO_GPIO, SERVO3_PIN);digitalHi(SERVO_GPIO, SERVO4_PIN);

#endif

/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32F10x Standard Peripherals Library
**
**  Distribution: The file is distributed �as is,� without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include "conf.h"


/* Private typedef */
/* Private define  */

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

u16 timercmd=0;


void hwInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
//    bool err = false;

    systemInit();

    /* Enable clocks for on stuff we use */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* �������� ������ ����������� */
    GPIO_InitStructure.GPIO_Pin   = LED_ERR_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LED_ERR_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LED_STS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LED_STS_GPIO, &GPIO_InitStructure);

	// ����������� ����� �����-������
    GPIO_InitStructure.GPIO_Pin = SERVO1_PIN | SERVO2_PIN | SERVO3_PIN | SERVO4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SERVO_GPIO , &GPIO_InitStructure);

    uartInit(9600);

    i2c_OLED_init();
}
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
//  int i = 0;

  /**
  *  IMPORTANT NOTE!
  *  The symbol VECT_TAB_SRAM needs to be defined when building the project
  *  if code has been located to RAM and interrupts are used. 
  *  Otherwise the interrupt table located in flash will be used.
  *  See also the <system_*.c> file and how the SystemInit() function updates 
  *  SCB->VTOR register.  
  *  E.g.  SCB->VTOR = 0x20000000;  
  */
  hwInit();

  /* TODO - Add your application code here */

  /* Infinite loop */
  while (1)
  {
	  if(rx_buffer_overflow)
	  {
		rx_buffer_overflow=0;
		if(rx_buffer[0]==0x0A) // FORWARD
		{
			timercmd=100;
			ROBOT_FORWARD;
		}
		if(rx_buffer[0]==0x0B) // BACKWARD
		{
			timercmd=100;
			ROBOT_BACKWARD;

		}
		if(rx_buffer[0]==0x0C) // LEFT
		{
			timercmd=100;
			ROBOT_LEFT;
		}
		if(rx_buffer[0]==0x0D) // RIGHT
		{
			timercmd=100;
			ROBOT_RIGHT;
		}
		if(rx_buffer[0]==0x0E) // STOP
		{
			timercmd=100;
			ROBOT_STOP;
		}
	  }
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}

#ifdef USE_SEE
#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval sEE_FAIL.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Return with error code */
  return sEE_FAIL;
}
#endif
#endif /* USE_SEE */


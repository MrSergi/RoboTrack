#include "conf.h"

// current uptime for 1kHz systick timer. will rollover after 49 days. hopefully we won't care.
volatile uint32_t sysTickUptime = 0;
u32 tim3_cnt = 0;

// SysTick
void SysTick_Handler(void)
{
    sysTickUptime++;
}

// TIM3 tick
void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	if(++tim3_cnt == 200) // период 10 мс
	{
		digitalToggle(LED_ERR_GPIO, LED_ERR_PIN);
		tim3_cnt = 0;
	}

	if((--timercmd)==0x00)
	{
		ROBOT_STOP;
	}
}

/*
 * 	Configure TIM3
 */
void TimerConfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 	= 1000 - 1; // timer update period 1 milisecond
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;	// timers run at 1 MHz
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	/* NVIC Configuration */
	NVIC_InitStructure.NVIC_IRQChannel 					 = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// Return system uptime in microseconds (rollover in 70minutes)
uint32_t micros(void)
{
	register uint32_t ms, cycle_cnt;
    do {
        ms = sysTickUptime;
        cycle_cnt = SysTick->VAL;
    } while (ms != sysTickUptime);
    return (ms * 1000) + (72000 - cycle_cnt) / 72;
}

// Return system uptime in milliseconds (rollover in 49 days)
uint32_t millis(void)
{
    return sysTickUptime;
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;
  // SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------
  // RCC system reset(for debug purpose)
  RCC_DeInit();

  // Enable HSE
  RCC_HSEConfig(RCC_HSE_ON);

  // Wait till HSE is ready
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
  // Enable Prefetch Buffer
      FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

  // Flash 2 wait state
  FLASH_SetLatency(FLASH_Latency_2);

  // HCLK = SYSCLK
  RCC_HCLKConfig(RCC_SYSCLK_Div1);

  // PCLK2 = HCLK
  RCC_PCLK2Config(RCC_HCLK_Div1);

  // PCLK1 = HCLK/2
  RCC_PCLK1Config(RCC_HCLK_Div2);

  #ifdef STM32F10X_CL
  // Configure PLLs *********************************************************
  // PLL2 configuration: PLL2CLK = (HSE / 4) * 8 = 32 MHz
  RCC_PREDIV2Config(RCC_PREDIV2_Div2);
  RCC_PLL2Config(RCC_PLL2Mul_8);

  // Enable PLL2
  RCC_PLL2Cmd(ENABLE);

  // Wait till PLL2 is ready
  while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
  {}

  // PLL configuration: PLLCLK = (HSE / 2) * 9 = 72 MHz

//    		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div4);
  RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div2);
  RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
  #else
  // PLLCLK = 8MHz * 9 = 72 MHz
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
  #endif

  // Enable PLL
  RCC_PLLCmd(ENABLE);

  // Wait till PLL is ready
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }

  // Select PLL as system clock source
//    	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  // Wait till PLL is used as system clock source
  while(RCC_GetSYSCLKSource() != 0x08)
  {
  }
  }
  else
  {
          // If HSE fails to start-up, the application will have wrong clock configuration.
  // User can add here some code to deal with this error

  // Go to infinite loop
  while (1);
  }

}

void systemInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_Configuration();

	/* Enable PWR and BKP clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enable clocks for on stuff we use */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA |
    		RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    /* Make all GPIO in by default to save power and reduce noise */
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure TIM3 */
    TimerConfig();
    // SysTick
    SysTick_Config(SystemCoreClock / 1000);
}

#if 1
void delayMicroseconds(uint32_t us)
{
    uint32_t now = micros();
    while (micros() - now < us);
}
#else
void delayMicroseconds(uint32_t us)
{
    uint32_t elapsed = 0;
    uint32_t lastCount = SysTick->VAL;

    for (;;) {
        register uint32_t current_count = SysTick->VAL;
        uint32_t elapsed_us;

        // measure the time elapsed since the last time we checked
        elapsed += current_count - lastCount;
        lastCount = current_count;

        // convert to microseconds
        elapsed_us = elapsed / usTicks;
        if (elapsed_us >= us)
            break;

        // reduce the delay by the elapsed time
        us -= elapsed_us;

        // keep fractional microseconds for the next iteration
        elapsed %= usTicks;
    }
}
#endif

void delay(uint32_t ms)
{
    while (ms--)
        delayMicroseconds(1000);
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
		digitalToggle(LED_ERR_GPIO, LED_ERR_PIN);
		simple_delay_ms(100);
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
		digitalToggle(LED_ERR_GPIO, LED_ERR_PIN);
		simple_delay_ms(100);
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
		digitalToggle(LED_ERR_GPIO, LED_ERR_PIN);
		simple_delay_ms(100);
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
		digitalToggle(LED_ERR_GPIO, LED_ERR_PIN);
		simple_delay_ms(100);
	}
}


/*
 * uart1.c
 *
 *  Created on: 02.06.2013
 *      Author: ��������� ��������
 */

#include "drv_uart.h"

/* --------------------------------- USART1 ------------------------------------- */

volatile uint8_t txBuffer[UART_BUFFER_SIZE];
uint32_t txBufferTail = 0;
uint32_t txBufferHead = 0;

// ����� �� �����
char    rx_buffer[UART_BUFFER_SIZE];
uint16_t   rx_wr_index=0, //������ ������ ������ (���� ������ ������)
                    rx_rd_index=0, //������ ������ ������ (������ ������ ������)
                    rx_counter=0; //���������� ������ � ������
uint8_t    rx_buffer_overflow=0; //���������� � ������������ ������

// ����� �� ��������
uint8_t   tx_buffer[UART_BUFFER_SIZE];
uint16_t  tx_wr_index=0, //������ ������ ������ (���� ������ ������)
                   tx_rd_index=0, //������ ������ ������ (������ ������ ������)
                   tx_counter=0; //���������� ������ � ������


static void uartOpen(uint32_t speed)
{
    USART_InitTypeDef USART_InitStructure;

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate 			  = speed;
    USART_InitStructure.USART_WordLength 		  = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits 			  = USART_StopBits_1;
    USART_InitStructure.USART_Parity 			  = USART_Parity_No;
    USART_InitStructure.USART_Mode 				  = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void uartInit(uint32_t speed)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel 					 = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ��������� ����� UART1_TX
    GPIO_InitStructure.GPIO_Pin 	= UART1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_Init(UART1_TX_GPIO, &GPIO_InitStructure);

    // ��������� ����� UART1_RX
    GPIO_InitStructure.GPIO_Pin 	= UART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
    GPIO_Init(UART1_RX_GPIO, &GPIO_InitStructure);

    uartOpen(speed);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//    uart1Callback = func;
}

void uartChangeBaud(uint32_t speed)
{
    uartOpen(speed);
}

void uartWrite(uint8_t ch)
{
    txBuffer[txBufferHead] = ch;
    txBufferHead = (txBufferHead + 1) % UART_BUFFER_SIZE;

    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

bool uartTransmitEmpty(void)
{
    return txBufferTail == txBufferHead;
}

void USART1_IRQHandler(void)
{
//    uint16_t SR = USART1->SR;
//    char bbb=0;

//    if (SR & USART_IT_RXNE)
//    {
//        if (uart1Callback) uart1Callback(USART_ReceiveData(USART1));
//    	bbb=USART_ReceiveData(USART1);

//    	if(bbb=='O')
//    		LED_OFF;
//    }
//    if (SR & USART_FLAG_TXE)
//    {
//        if (tx1BufferTail != tx1BufferHead)
//        {
//            USART1->DR = tx1Buffer[tx1BufferTail];
//            tx1BufferTail = (tx1BufferTail + 1) % UART1_BUFFER_SIZE;
//        }
//        else
//        {
//            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//        }
//    }
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//    {
//    	bbb=USART_ReceiveData(USART1);
//    	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//    }
//        uint16_t SR = USART1->SR;
//
//        if (SR & USART_IT_RXNE)
//        {
//            bbb = USART_ReceiveData(USART1);
//            if(bbb == 0x09)
//            	LED_OFF;
//
//        }

	  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) //���������� �� ������ ������
	  {
	    if ((USART1->SR & (USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_ORE)) == 0) //��������� ��� �� ������
	    {
	      rx_buffer[rx_wr_index++]= (uint8_t) (USART_ReceiveData(USART1)& 0xFF); //��������� ������ � �����, ������������� ����� ������
	      if (rx_wr_index == UART_BUFFER_SIZE) rx_wr_index=0; //���� �� �����
	      if (++rx_counter == UART_BUFFER_SIZE) //������������ ������
	      {
	        rx_counter=0; //�������� ������� (������� ��� ������)
	        rx_buffer_overflow=1;  //�������� � ������������
	      }
	    }
	  }
}

void USART_OutChar(char ch)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // Wait for the TX buffer to be EMPTY FFS

  USART_SendData(USART1, ch);
}


void USART_OutString(char *s)
{
  while(*s)
    USART_OutChar(*s++);
}


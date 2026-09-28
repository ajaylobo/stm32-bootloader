/*
 * uart_driver.c
 *
 *  Created on: 06-Aug-2026
 *      Author: ajayd
 */

#include "stm32f4xx.h"
#include "uart_driver.h"
#include "ring_buffer.h"


static RingBuffer_t uart_rx_buffer;

static volatile bool uart_rx_overflow;

static void UART_SetBaudRate(uint32_t pclk, uint32_t baudrate);

void UART_Init(void)
{
	RingBuffer_Init(&uart_rx_buffer);
	uart_rx_overflow = false;

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Enable USART2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;


    /* PA2 and PA3 -> Alternate Function mode */
    GPIOA->MODER &= ~((3U << 4U) | (3U << 6U));
    GPIOA->MODER |=  ((2U << 4U) | (2U << 6U));


    /* PA2 and PA3 -> AF7 (USART2) */
    GPIOA->AFR[0] &= ~((0xFU << 12U) | (0xFU << 8U));
    GPIOA->AFR[0] |=  ((7U << 12U) | (7U << 8U));


    /* Configure baud rate */
    UART_SetBaudRate(42000000U, 115200U);


    /* Configure USART */
    USART2->CR1 = USART_CR1_RE |
                  USART_CR1_TE|
                  USART_CR1_RXNEIE;

    /* 1 stop bit, default configuration */
    USART2->CR2 = 0U;

    /* No DMA, no flow control */
    USART2->CR3 = 0U;


    /* Enable USART2 interrupt in NVIC */
    NVIC_EnableIRQ(USART2_IRQn);
    

    /* Enable USART2 */
    USART2->CR1 |= USART_CR1_UE;

}


static void UART_SetBaudRate(uint32_t pclk, uint32_t baudrate)
{
    uint32_t divisor;
    uint32_t mantissa;
    uint32_t remainder;
    uint32_t fraction;

    divisor = 16U * baudrate;

    mantissa = pclk / divisor;

    remainder = pclk % divisor;

    fraction = (remainder * 16U + (divisor / 2U)) / divisor;

    USART2->BRR = (mantissa << 4U) | fraction;
}

void USART2_IRQHandler(void)
{
	if ((USART2->SR & USART_SR_RXNE) != 0U)
	{
		uint8_t byte = (uint8_t)USART2->DR;
		if(!RingBuffer_Push(&uart_rx_buffer, byte))
		{
			uart_rx_overflow = true;
		}
	}
}

bool UART_ReadByte(uint8_t *byte)
{
	return RingBuffer_Pop(&uart_rx_buffer, byte);
}


bool UART_HasRxOverflow(void)
{
    return uart_rx_overflow;
}

void UART_ClearRxOverflow(void)
{
    uart_rx_overflow = false;
}


bool UART_WriteByte(uint8_t byte)
{
	while((USART2->SR & USART_SR_TXE) == 0U)
	{
		/* Wait till the transfer data register is empty */
	}
	USART2->DR = byte;
	return true;
}


bool UART_WriteBuffer(const uint8_t *data, uint16_t length)
{
	bool status = false;
	if((data == NULL) || (length == 0U))
	{
		return status;
	}

	for(uint16_t i = 0U; i < length; i++)
	{
		 status = UART_WriteByte(data[i]);
		 if(status == false)
		 {
			 return status;
		 }
	}
	return true;
}

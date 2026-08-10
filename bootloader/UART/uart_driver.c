/*
 * uart_driver.c
 *
 *  Created on: 06-Aug-2026
 *      Author: ajayd
 */

#include "stm32f4xx.h"
#include "uart_driver.h"


void UART_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //enable AHB bus clock
	RCC->APB2ENR |= RCC_APB1ENR_USART2EN; //enable



	/* PA2 and PA3 -> Alternate Function mode */
	GPIOA->MODER &= ~((3U << 4U) | (3U << 6U));  // clear the field for safety and then configure
	GPIOA->MODER |= ((2U << 4U) | (2U << 6U));

	/* PA2 and PA3 -> AF7 (USART2) */
	GPIOA->AFR[0] &= ~((0xFU << 12U) | (0xFU << 8U));
	GPIOA->AFR[0] |= ((7U << 12U) | 7U << 8U);
}

bool UART_ReadByte(uint8_t *byte);
bool UART_WriteByte(uint8_t *byte);
bool UART_WriteBuffer(const uint8_t *data, uint16_t length);
void UART2_IRQHandler(void);

static void UART_SetBaudRate(uint32_t pclk, uint32_t baudrate)
{
	// USARTDIV = 42,000,000 / (16 × 115,200)

	USART2->BRR = 0U;

}

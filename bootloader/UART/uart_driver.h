/*
 * uart_driver.h
 *
 *  Created on: 06-Aug-2026
 *      Author: ajayd
 */

#ifndef UART_UART_DRIVER_H_
#define UART_UART_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

void UART_Init(void);
bool UART_ReadByte(uint8_t *byte);
bool UART_WriteByte(uint8_t *byte);
bool UART_WriteBuffer(const uint8_t *data, uint16_t length);
void UART2_IRQHandler(void);
bool UART_HasRxOverflow(void);
void UART_ClearRxOverflow(void);


#endif /* UART_UART_DRIVER_H_ */

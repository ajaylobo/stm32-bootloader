/*
 * jump.c
 *
 *  Created on: 16-Jul-2026
 *      Author: ajayd
 */


#include "jump.h"
#include "stm32f4xx.h"
#include "validation.h"


typedef void (*pFunction)(void);

void JumpToApplication(void)
{


	uint32_t app_msp = *(uint32_t *) APP_ADDRESS;
	uint32_t app_reset = *(uint32_t *) (APP_ADDRESS + 4);
	pFunction app_entry = (pFunction)app_reset;

	__disable_irq();

	/*Stop SysTick*/
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	HAL_RCC_DeInit();
	HAL_DeInit();

	__set_MSP(app_msp);

	SCB->VTOR = APP_ADDRESS;


	__enable_irq();
	app_entry();



}

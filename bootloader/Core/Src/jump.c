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

	/* Disable and clear NVIC interrupts */
	for(uint32_t i=0; i<3; i++)

	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}

	/* reset, disable, and clear the SysTick */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL  = 0;

	HAL_RCC_DeInit();
	HAL_DeInit();

	/* Update vector table */
	SCB->VTOR = APP_ADDRESS;

	/* Ensure the write completed */
	__DSB();

	/* Flush pipeline */
	__ISB();

	/* Switch to application's stack */
	__set_MSP(app_msp);

	/* Re-enable interrupts */
	__enable_irq();

	/* Jump to application */
	app_entry();


}

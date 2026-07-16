#include "validation.h"
#include "stm32f4xx.h"


bool IsApplicationValid(void)
{
	uint32_t app_msp = *(uint32_t *) APP_ADDRESS;
	uint32_t app_reset = *(uint32_t *) (APP_ADDRESS + 4);

	if(app_msp == EMPTY_FLASH)
		return false;

	if(app_msp < SRAM_START || app_msp > SRAM_END)
		return false;

	if(app_reset == EMPTY_FLASH)
		return false;

	if(app_reset < APP_RESET || app_reset > FLASH_END)
		return false;

	return true;
}

/*
 * flash_test.c
 *
 *  Created on: 04-Sept-2026
 *      Author: ajayd
 */


#include "flash_test.h"
#include "flash.h"
#include "validation.h"


Flash_Status_t status;

void flash_test()
{


	uint32_t test_address = APP_ADDRESS_START;
	uint32_t test_data = 0x12345678U;

	status = Flash_EraseSector(FLASH_SECTOR_4);
	if(status != FLASH_OK)
	{
		while(1);
	}

	status = Flash_ProgramWord(test_address, test_data);

	if(status != FLASH_OK)
	{
		while(1);
	}

	status = Flash_Verify(test_address, test_data);
	if(status != FLASH_OK)
	{
		while(1);
	}

	while(1);
}

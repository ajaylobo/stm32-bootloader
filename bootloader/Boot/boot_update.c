/*
 * boot_update.c
 *
 *  Created on: 04-Sept-2026
 *      Author: ajayd
 */

#include "boot_update.h"
#include "boot_protocol.h"
#include "flash.h"

static uint32_t firmware_size;
static uint32_t received_firmware_size;
static uint32_t current_flash_address;

BootUpdate_Status_t BootUpdate_Start(uint32_t new_firmware_size)
{
	if((firmware_size == 0U) || (firmware_size > APP_FLASH_SIZE))
	{
		return BOOT_UPDATE_ERROR_INVALID_SIZE;
	}

	Flash_Status_t status = Flash_EraseSector(FLASH_SECTOR_4);
	if(status != FLASH_OK)
	{
		return BOOT_UPDATE_ERROR_ERASE;
	}

	status = Flash_EraseSector(FLASH_SECTOR_5);
	if(status != FLASH_OK)
	{
		return BOOT_UPDATE_ERROR_ERASE;
	}

	status = Flash_EraseSector(FLASH_SECTOR_6);
	if(status != FLASH_OK)
	{
		return BOOT_UPDATE_ERROR_ERASE;
	}

	status = Flash_EraseSector(FLASH_SECTOR_7);
	if(status != FLASH_OK)
	{
		return BOOT_UPDATE_ERROR_ERASE;
	}

	firmware_size = new_firmware_size;
	received_firmware_size = 0U;
	current_flash_address = APP_FLASH_SIZE;

	return BOOT_UPDATE_OK;

}

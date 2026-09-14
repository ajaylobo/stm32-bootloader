/*
 * boot_update.c
 *
 *  Created on: 04-Sept-2026
 *      Author: ajayd
 */

#include "boot_update.h"
#include "flash.h"
#include "validation.h"
#include <stdbool.h>
#include <stddef.h>

static uint32_t firmware_size;
static uint32_t received_firmware_size;
static uint32_t current_flash_address;
static bool update_active;
static uint8_t pending_length;
static uint8_t pending_buffer[4];

BootUpdate_Status_t BootUpdate_Start(uint32_t new_firmware_size)
{
	if((new_firmware_size == 0U) || (new_firmware_size > APP_FLASH_SIZE))
	{
		update_active = false;
		return BOOT_UPDATE_ERROR_INVALID_SIZE;
	}

	Flash_Status_t status = Flash_EraseSector(FLASH_SECTOR_4);
	if(status != FLASH_OK)
	{
		update_active = false;
		return BOOT_UPDATE_ERROR_ERASE;
	}

	status = Flash_EraseSector(FLASH_SECTOR_5);
	if(status != FLASH_OK)
	{
		update_active = false;
		return BOOT_UPDATE_ERROR_ERASE;
	}

	status = Flash_EraseSector(FLASH_SECTOR_6);
	if(status != FLASH_OK)
	{
		update_active = false;
		return BOOT_UPDATE_ERROR_ERASE;
	}

	status = Flash_EraseSector(FLASH_SECTOR_7);
	if(status != FLASH_OK)
	{
		update_active = false;
		return BOOT_UPDATE_ERROR_ERASE;
	}

	firmware_size = new_firmware_size;
	received_firmware_size = 0U;
	current_flash_address = APP_ADDRESS_START;
	update_active = true;
	pending_length = 0U;

	return BOOT_UPDATE_OK;

}

BootUpdate_Status_t BootUpdate_Write(const uint8_t *data, uint16_t length)
{
	if(!update_active)
	{
		return BOOT_UPDATE_ERROR_NOT_ACTIVE;
	}

	if((data == NULL) || (length == 0))
	{
		return BOOT_UPDATE_ERROR_SIZE_MISMATCH;
	}

	if(received_firmware_size > firmware_size)
	{
		return BOOT_UPDATE_ERROR_SIZE_MISMATCH;
	}

	if((uint32_t)length > (firmware_size - received_firmware_size))
	{
		return BOOT_UPDATE_ERROR_SIZE_MISMATCH;
	}



}

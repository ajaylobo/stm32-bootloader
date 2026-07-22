/*
 * flash.h
 *
 *  Created on: 21-Jul-2026
 *      Author: ajayd
 */

#ifndef FLASH_FLASH_H_
#define FLASH_FLASH_H_

#include <stdint.h>

#define FLASH_ERASE_TIMEOUT    100000U

typedef enum {
	FLASH_OK = 0,
    FLASH_ERROR_TIMEOUT,
    FLASH_ERROR_WRITE_PROTECT,
    FLASH_ERROR_ALIGNMENT,
    FLASH_ERROR_PARALLELISM,
    FLASH_ERROR_SEQUENCE,
    FLASH_ERROR_OPERATION,
    FLASH_ERROR_LOCK_FAILED,
    FLASH_ERROR_UNLOCK_FAILED,
    FLASH_ERROR_INVALID_SECTOR
}Flash_Status_t;

typedef enum{
	FLASH_SECTOR_0 = 0,
	FLASH_SECTOR_1,
	FLASH_SECTOR_2,
	FLASH_SECTOR_3,
	FLASH_SECTOR_4,
	FLASH_SECTOR_5,
	FLASH_SECTOR_6,
	FLASH_SECTOR_7
}Flash_Sector_t;

Flash_Status_t Flash_Unlock(void);
Flash_Status_t Flash_Lock(void);
Flash_Status_t Flash_WaitForReady(uint32_t timeout);
Flash_Status_t Flash_EraseSector(Flash_Sector_t sector);
Flash_Status_t Flash_ProgramWord(uint32_t address, uint32_t data);
Flash_Status_t Flash_Verify(uint32_t address, uint32_t data);

uint32_t Flash_GetTick(void);

#endif /* FLASH_FLASH_H_ */

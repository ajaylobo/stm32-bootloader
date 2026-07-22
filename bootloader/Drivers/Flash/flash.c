/*
 * flash.c
 *
 *  Created on: 21-Jul-2026
 *      Author: ajayd
 */


#include "flash.h"
#include "stm32f4xx.h"

static Flash_Status_t Flash_CheckErrors(void);


Flash_Status_t Flash_Unlock(void)
{
    /* Unlock Flash only if it is currently locked */
    if ((FLASH->CR & FLASH_CR_LOCK) != 0U)
    {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;

        /* Verify unlock operation */
        if ((FLASH->CR & FLASH_CR_LOCK) != 0U)
        {
            return FLASH_ERROR_UNLOCK_FAILED;
        }
    }

    return FLASH_OK;
}

Flash_Status_t Flash_Lock(void)
{
    /* Lock flash only if it is currently unlocked */
    if ((FLASH->CR & FLASH_CR_LOCK) == 0U)
    {
        FLASH->CR |= FLASH_CR_LOCK;

        /* Verify lock operation */
        if ((FLASH->CR & FLASH_CR_LOCK) == 0U)
        {
            return FLASH_ERROR_LOCK_FAILED;
        }
    }

    return FLASH_OK;
}

Flash_Status_t Flash_WaitForReady(uint32_t timeout)
{
	uint32_t start = Flash_GetTick();

	while((FLASH->SR & FLASH_SR_BSY) != 0U)
	{
		if((Flash_GetTick() - start) >= timeout)
		{
			return FLASH_ERROR_TIMEOUT;
		}
	}

	Flash_Status_t status = Flash_CheckErrors();

	if(status != FLASH_OK)
	{
	    return status;
	}

	return FLASH_OK;
}


static Flash_Status_t Flash_CheckErrors()
{
	if((FLASH->SR & FLASH_SR_WRPERR) != 0U)
	{
		FLASH->SR = FLASH_SR_WRPERR;
		return FLASH_ERROR_WRITE_PROTECT;
	}

	if((FLASH->SR & FLASH_SR_PGAERR) != 0U)
	{
		FLASH->SR = FLASH_SR_PGAERR;
		return FLASH_ERROR_ALIGNMENT;
	}


	if((FLASH->SR & FLASH_SR_PGPERR) != 0U)
	{
		FLASH->SR = FLASH_SR_PGPERR;
		return FLASH_ERROR_PARALLELISM;
	}

	if((FLASH->SR & FLASH_SR_PGSERR) != 0U)
	{
		FLASH->SR = FLASH_SR_PGSERR;
		return FLASH_ERROR_SEQUENCE;

	}

	if((FLASH->SR & FLASH_SR_OPERR) != 0U)
	{
		FLASH->SR = FLASH_SR_OPERR;
		return FLASH_ERROR_OPERATION;
	}

	if ((FLASH->SR & FLASH_SR_EOP) != 0U)
	{
	    FLASH->SR = FLASH_SR_EOP;
	}

	return FLASH_OK;

}


Flash_Status_t Flash_EraseSector(Flash_Sector_t sector)
{
	Flash_Status_t status = FLASH_OK;
	Flash_Status_t lock_status;

	if(FLASH_SECTOR_7 < sector)
	{
		return FLASH_ERROR_INVALID_SECTOR;
	}

	status = Flash_Unlock();
	if(FLASH_OK != status)
	{
		return status;
	}

	status = Flash_WaitForReady(FLASH_ERASE_TIMEOUT);
	if(FLASH_OK != status)
	{
		goto cleanup;
	}

	/* clear program bit before erase*/
	FLASH->CR &= ~FLASH_CR_PG;

	/* Select sector erase */
	FLASH->CR |= FLASH_CR_SER;

	/* clear and set the sector number*/
	FLASH->CR &= ~FLASH_CR_SNB;
	FLASH->CR |= ((uint32_t)sector << FLASH_CR_SNB_Pos);

	FLASH->CR |= FLASH_CR_STRT;

	status = Flash_WaitForReady(FLASH_ERASE_TIMEOUT);

	FLASH->CR &= ~(FLASH_CR_SER | FLASH_CR_SNB);

	cleanup:
	lock_status = Flash_Lock();

	if((status == FLASH_OK) &&
	   (lock_status != FLASH_OK))
	{
		status = lock_status;
	}

	return status;
}

uint32_t Flash_GetTick(void)
{
    return HAL_GetTick();
}

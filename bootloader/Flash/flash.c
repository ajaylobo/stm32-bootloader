/*
 * flash.c
 *
 *  Created on: 21-Jul-2026
 *      Author: ajayd
 */


#include "flash.h"
#include "validation.h"
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


static Flash_Status_t Flash_CheckErrors(void)
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

Flash_Status_t Flash_ProgramWord(uint32_t address, uint32_t data)
{
    Flash_Status_t status = FLASH_OK;
    Flash_Status_t lock_status;

    /*
     * TODO:
     * 1. Validate address range
     * 2. Validate 4-byte alignment
     * 3. Unlock Flash
     * 4. Wait until ready
     * 5. Clear previous status flags
     * 6. Clear SER (safety)
     * 7. Configure PSIZE = Word
     * 8. Set PG
     * 9. Program:
     *      *(volatile uint32_t *)address = data;
     * 10. Wait until ready
     * 11. Clear PG
     * 12. Lock Flash
     * 13. Return status
     */

    if ((address < FLASH_START) || (address > FLASH_END))
    	return FLASH_ERROR_FLASH_ADDRESS;

    if((address & 0x3U) != 0U)
    	return FLASH_ERROR_ALIGNMENT;

    status = Flash_Unlock();
	if(status != FLASH_OK)
		return status;
	
	status = Flash_WaitForReady(FLASH_PROGRAM_TIMEOUT);
	if(FLASH_OK != status)
	{
		goto cleanup;
	}

	/* clear SER for safety */
	FLASH->CR &= ~FLASH_CR_SER;

	/* configure PSIZE to 32bit word */
	FLASH->CR &= ~FLASH_CR_PSIZE;
	FLASH->CR |= FLASH_PSIZE_WORD;

	/*  set PG bit */
	FLASH->CR |= FLASH_CR_PG;

	*(volatile uint32_t *)address = data;

	status = Flash_WaitForReady(FLASH_PROGRAM_TIMEOUT);
	if(FLASH_OK != status)
	{
		goto cleanup;
	}


	cleanup:
	FLASH->CR &= ~FLASH_CR_PG;
	lock_status = Flash_Lock();
	if((status == FLASH_OK) &&
	   (lock_status != FLASH_OK))
	{
		status = lock_status;
	}

	return status;

}

Flash_Status_t Flash_Verify(uint32_t address, uint32_t data)
{
	if (*(volatile uint32_t *)address == data)
	{
	    return FLASH_OK;
	}

	return FLASH_ERROR_VERIFY;
}

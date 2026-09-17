/*
 * boot_update.h
 *
 *  Created on: 04-Sept-2026
 *      Author: ajayd
 */

#ifndef BOOT_BOOT_UPDATE_H_
#define BOOT_BOOT_UPDATE_H_
#include<stdint.h>

typedef enum {
	BOOT_UPDATE_OK = 0,
	BOOT_UPDATE_ERROR_INVALID_SIZE,
	BOOT_UPDATE_ERROR_ERASE,
	BOOT_UPDATE_ERROR_SIZE_MISMATCH,
	BOOT_UPDATE_ERROR_NOT_ACTIVE,
	BOOT_UPDATE_ERROR_PROGRAM,
	BOOT_UPDATE_ERROR_INVALID_APP
}BootUpdate_Status_t;


BootUpdate_Status_t BootUpdate_Start(uint32_t new_firmware_size);
BootUpdate_Status_t BootUpdate_Write(const uint8_t *data, uint16_t length);
BootUpdate_Status_t BootUpdate_End(void);


#endif /* BOOT_BOOT_UPDATE_H_ */

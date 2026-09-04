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
	BOOT_UPDATE_ERROR_ERASE
}BootUpdate_Status_t;


BootUpdate_Status_t BootUpdate_Start(uint32_t firmware_size);


#endif /* BOOT_BOOT_UPDATE_H_ */

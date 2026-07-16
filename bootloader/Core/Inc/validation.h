#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>

#define FLASH_START     0x08000000U
//#define FLASH_END       0x08080000U

#define SRAM_START      0x20000000U
#define SRAM_END        0x20020000U

#define APP_ADDRESS     0x08010000U
#define APP_RESET       0x08010001U
#define EMPTY_FLASH     0xFFFFFFFFU

bool IsApplicationValid(void);

#endif

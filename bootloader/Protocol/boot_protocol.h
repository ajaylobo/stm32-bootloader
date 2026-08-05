/*
 * boot_protocol.h
 *
 *  Created on: 05-Aug-2026
 *      Author: ajayd
 */

#ifndef INC_BOOT_PROTOCOL_H_
#define INC_BOOT_PROTOCOL_H_


#include<stdint.h>

#define BOOT_SOF	(0xAAU)
#define BOOT_MAX_PAYLOAD_SIZE	(128U)
#define BOOT_PROTOCOL_VERSION    (1U)

typedef enum {
	BOOT_CMD_START_UPDATE = 0x01,
	BOOT_CMD_DATA = 0x02,
	BOOT_CMD_END_UPDATE = 0x03,
	BOOT_CMD_GET_VERSION = 0x04,
	BOOT_CMD_ACK = 0x05,
	BOOT_CMD_NACK = 0x06
}Boot_Command_t;



typedef struct {
	uint8_t sof;
	Boot_Command_t command;
	uint16_t length;
	uint8_t payload[BOOT_MAX_PAYLOAD_SIZE];
	uint16_t crc;
	uint8_t protocol_version;
}Boot_Packet_t;

typedef enum {
	BOOT_PACKET_OK = 0,
	BOOT_PACKET_INCOMPLETE,
	BOOT_PACKET_CRC_ERROR,
	BOOT_PACKET_TIMEOUT,
	BOOT_PACKET_INVALID_LENGTH,
	BOOT_PACKET_INVALID_SOF
}Packet_Status_t;

typedef struct {
	uint32_t firmware_size;
	uint32_t firmware_crc;
	uint32_t firmware_version;
	uint32_t hardware_id;
}Boot_StartUpdatePayload_t;

typedef struct {
	Flash_Status_t error;
}Boot_NackPayload_t;

#endif /* INC_BOOT_PROTOCOL_H_ */

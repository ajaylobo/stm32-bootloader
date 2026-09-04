/*
 * boot_protocol.c
 *
 *  Created on: Aug 24, 2026
 *      Author: ajay
 */

#include <stdbool.h>
#include "boot_protocol.h"
#include "uart_driver.h"

BootParser_State_t parser_state = BOOT_STATE_WAIT_SOF;
Boot_Packet_t current_packet;
uint16_t payload_index = 0;
uint16_t current_crc;

void BootProtocol_Process()
{
	uint8_t byte;
	if(!UART_ReadByte(&byte))
	{
		return;
	}
	switch (parser_state)
	{

	case BOOT_STATE_WAIT_SOF:
		if(byte == BOOT_SOF)
		{
			current_packet.sof = byte;
			parser_state = BOOT_STATE_WAIT_VERSION;
			payload_index = 0;
			current_crc = 0xFFFFU;
		}
		break;

	case BOOT_STATE_WAIT_VERSION:
		if(byte == BOOT_PROTOCOL_VERSION)
		{
			current_packet.protocol_version = byte;
			parser_state = BOOT_STATE_WAIT_COMMAND;
			current_crc = Calculate_CRC(current_crc, byte);
		}
		else{
			parser_state = BOOT_STATE_WAIT_SOF;
			//TODO: NACK response must be generated.
		}
		break;

	case BOOT_STATE_WAIT_COMMAND:
		if((byte == BOOT_CMD_START_UPDATE) ||
				(byte == BOOT_CMD_DATA) ||
				(byte == BOOT_CMD_END_UPDATE) ||
				(byte == BOOT_CMD_GET_VERSION))
		{
			current_packet.command = byte;
			parser_state = BOOT_STATE_WAIT_LENGTH_LOW;
			current_crc = Calculate_CRC(current_crc, byte);
		}
		else{
			parser_state = BOOT_STATE_WAIT_SOF;
		}
		break;

	case BOOT_STATE_WAIT_LENGTH_LOW:
		current_packet.length = byte;
		parser_state = BOOT_STATE_WAIT_LENGTH_HIGH;
		current_crc = Calculate_CRC(current_crc, byte);
		break;

	case BOOT_STATE_WAIT_LENGTH_HIGH:
		current_packet.length |= ((uint16_t)byte << 8U);
		current_crc = Calculate_CRC(current_crc, byte);
		if(current_packet.length > BOOT_MAX_PAYLOAD_SIZE)
		{
			parser_state = BOOT_STATE_WAIT_SOF;
		}
		else if(current_packet.length == 0)
		{
			parser_state = BOOT_STATE_WAIT_CRC_LOW;
		}
		else
		{
			parser_state = BOOT_STATE_WAIT_PAYLOAD;
		}
		break;

	case BOOT_STATE_WAIT_PAYLOAD:
		current_packet.payload[payload_index] = byte;
		current_crc = Calculate_CRC(current_crc, byte);
		payload_index++;
		if(payload_index == current_packet.length)
		{
			parser_state = BOOT_STATE_WAIT_CRC_LOW;
		}
		break;

	case BOOT_STATE_WAIT_CRC_LOW:
		current_packet.crc = byte;
		parser_state = BOOT_STATE_WAIT_CRC_HIGH;
		break;

	case BOOT_STATE_WAIT_CRC_HIGH:
		current_packet.crc |= ((uint16_t)byte << 8U);
		parser_state = BOOT_STATE_WAIT_SOF;
		if(current_crc != current_packet.crc)
		{
			//TODO: NACK response must be generated.
			return;
		}
		if(current_packet.command == BOOT_CMD_START_UPDATE)
		{
			if(current_packet.length != 4U)
			{
				//TODO: NACK invalid start_update length
				return;
			}

			firmware_size = 0U;
			for(uint8_t i = 0U; i < 4U; i++)
			{
				firmware_size |= ((uint32_t)current_packet.payload[i] << (i*8U));
			}

			if(firmware_size > APP_FLASH_SIZE)
			{
				//TODO: NACK FW is too large
				return;
			}
			received_firmware_size = 0U;
		}
		break;

	default:
		parser_state = BOOT_STATE_WAIT_SOF;

	}

}

uint16_t Calculate_CRC(uint16_t crc, uint8_t byte)
{
	uint8_t loop = 8U;
	crc ^= ((uint16_t)byte << 8U);
	while(loop)
	{
		bool MSB = (crc & 0x8000U) != 0U;
		crc = crc << 1U;
		if(MSB)
		{
			crc ^= 0x1021U;
		}
		loop--;
	}
	return crc;
}

















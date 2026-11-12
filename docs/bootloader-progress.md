# Bootloader Progress

## Milestone 1 - Bootloader Foundation

Completed:
- Created bootloader project
- Created application project
- Relocated application to 0x08010000
- Implemented application validation
- Implemented MSP switching
- Implemented VTOR relocation
- Successfully transferred execution from bootloader to application

Key Learning:
- Cortex-M reset sequence
- Vector table
- Linker script modifications
- Startup code
- MSP and VTOR
- PRIMASK and interrupt masking
- Debugging with PC, MSP, and VTOR registers

## Milestone 2 - UART Firmware Update

Implemented:
- USART2 receive/transmit driver using PA2 (TX) and PA3 (RX) at 115200 baud
- Framed update protocol with a 128-byte maximum payload and packet CRC
- ACK/NACK responses for update start, data, and completion
- Flash erase/program flow for application sectors 4 and 5
- Firmware-size tracking and basic application vector validation before reboot
- Python serial programmer in `Programmer/stm_programmer.py`
- Three-second update-request window before normal application handoff

Current limits:
- Application image size is limited to 192 KiB.
- The packet CRC is not a whole-image checksum or a security feature.
- `GET_VERSION` is defined in the protocol but does not have a response implementation.
- There is no packet retry, update timeout after an update has started, rollback, or power-loss recovery.

See [UART Firmware Update](uart_firmware_update.md) for setup, packet details, and usage.
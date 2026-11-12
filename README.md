# STM32 Bootloader

A production-style bootloader developed from scratch for the **STM32F446RE** using Embedded C and STM32CubeIDE.

The goal of this project is not just to build a working bootloader, but to understand every stage of the Cortex-M boot process, memory layout, startup code, vector table relocation, application handoff, and UART firmware update mechanism.

---

## Current Features

- ✅ Independent Bootloader Project
- ✅ Independent Application Project
- ✅ Application Relocation
- ✅ NVIC Cleanup
- ✅ Vector Table Relocation (VTOR)
- ✅ Main Stack Pointer (MSP) Switching
- ✅ Application Validation
- ✅ Bootloader → Application Handoff
- ✅ UART Firmware Transfer and Flash Upgrade
- ✅ Packet CRC and ACK/NACK Responses

---

## Memory Layout

| Region | Start Address | Size |
|---------|--------------|------|
| Bootloader | 0x08000000 | 64 KB |
| Application image | 0x08010000 | Up to 192 KB |
| Remaining Flash | 0x08040000 | 256 KB (not used by the current updater) |
| SRAM | 0x20000000 | 128 KB |

The application linker script is currently limited to 192 KB, matching the flash sectors erased by the update code.

---

## Boot Flow

```
Power On
    │
    ▼
Bootloader
    │
    ├── Receive START_UPDATE over USART2 within 3 seconds
    │       └── Erase application sectors, receive DATA packets,
    │           validate the application, then reset
    │
    └── No update request within 3 seconds
            ├── Valid application → JumpToApplication()
            └── Invalid application → Stay in bootloader
```

---

## UART Firmware Update

The included Python programmer transfers a raw application `.bin` over USART2 using a stop-and-wait packet protocol. It sends a start packet containing the image size, 128-byte-or-smaller data packets, and an end packet. The programmer checks ACK/NACK responses for each update command.

The current update limit is 192 KiB. USART2 is configured for 115200 baud on PA2 (TX) and PA3 (RX). The programmer defaults to `COM4`; edit `PORT` in `Programmer/stm_programmer.py` to select the serial port for your setup.

```powershell
python -m pip install pyserial
python Programmer\stm_programmer.py path\to\application.bin
```

The bootloader must already be programmed to the board. The application must be built for `0x08010000`. See [the UART firmware update guide](docs/uart_firmware_update.md) for wiring, packet format, workflow, and current limitations.

---

## Development Roadmap

### Completed

- [x] Independent bootloader and application projects
- [x] Application linker relocation, validation, and bootloader handoff
- [x] UART2 driver and packet protocol
- [x] Flash erase/program path and UART firmware update

### Planned

- [ ] Whole-image CRC validation
- [ ] Firmware metadata and version management
- [ ] Update recovery and rollback protection
- [ ] YMODEM support

---

## Hardware

- STM32F446RE Nucleo Board
- ST-Link V2-1

---

## Development Environment

- STM32CubeIDE
- STM32CubeProgrammer
- Git
- Embedded C

---

## License

MIT License

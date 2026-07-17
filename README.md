# STM32 Bootloader

A production-style STM32 bootloader developed from scratch on the STM32F446RE.

## Features

- ✅ Independent Bootloader
- ✅ Independent Application
- ✅ Application Relocation (0x08010000)
- ✅ Application Validation
- ✅ Vector Table Relocation (VTOR)
- ✅ Main Stack Pointer (MSP) Switching
- ✅ Bootloader → Application Handoff
- [ ] UART Firmware Update
- [ ] Flash Programming
- [ ] CRC Verification
- [ ] Metadata Support
- [ ] Version management
- [ ] Rollback protection

## Memory Layout

| Region | Address |
|---------|----------|
| Bootloader | 0x08000000 |
| Application | 0x08010000 |

## Hardware

- STM32F446RE Nucleo Board
- ST-Link V2-1

## Toolchain

- STM32CubeIDE
- STM32CubeProgrammer
- Git
- Ubuntu / Windows

## Current Status

This project currently supports booting a relocated application after validating its vector table and transferring execution by updating the VTOR and MSP.

Future work includes:

- UART firmware updates
- Flash programming
- CRC verification
- Metadata
- Version management
- Rollback protection
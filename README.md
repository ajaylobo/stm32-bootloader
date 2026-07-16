# STM32 Bootloader

A production-style STM32 bootloader developed from scratch on the STM32F446RE.

## Current Progress

- [x] Bootloader Project
- [x] Application Project
- [x] Application Relocation
- [x] Application Validation
- [ ] Jump to Application
- [ ] UART Firmware Update
- [ ] Flash Programming
- [ ] CRC Verification
- [ ] Metadata Support

## MCU

- STM32F446RE
- STM32CubeIDE
- STM32CubeProgrammer

## Memory Layout

| Region | Address |
|---------|----------|
| Bootloader | 0x08000000 |
| Application | 0x08010000 |
# STM32 Bootloader

A production-style bootloader developed from scratch for the **STM32F446RE** using Embedded C and STM32CubeIDE.

The goal of this project is not just to build a working bootloader, but to understand every stage of the Cortex-M boot process, memory layout, startup code, vector table relocation, application handoff, and firmware update mechanism.

---

## Current Features

- ✅ Independent Bootloader Project
- ✅ Independent Application Project
- ✅ Application Relocation
- ✅ Vector Table Relocation (VTOR)
- ✅ Main Stack Pointer (MSP) Switching
- ✅ Application Validation
- ✅ Bootloader → Application Handoff

---

## Memory Layout

| Region | Start Address | Size |
|---------|--------------|------|
| Bootloader | 0x08000000 | 64 KB |
| Application | 0x08010000 | Remaining Flash |
| SRAM | 0x20000000 | 128 KB |

---

## Boot Flow

```
Power On
    │
    ▼
Bootloader
    │
    ▼
Validate Application
    │
 ┌──┴──┐
 │     │
 ▼     ▼
Invalid Valid
 │      │
 │      ▼
 │  JumpToApplication()
 │      │
 ▼      ▼
Stay    Application
```

---

## Development Roadmap

### Milestone 1 (Completed)

- [x] Bootloader Project
- [x] Application Project
- [x] Linker Script Relocation
- [x] Application Validation
- [x] MSP Switching
- [x] VTOR Relocation
- [x] Successful Application Jump

### Upcoming

- [ ] NVIC Cleanup
- [ ] Flash Driver
- [ ] UART Driver
- [ ] UART Firmware Update
- [ ] CRC Verification
- [ ] Metadata
- [ ] Version Management
- [ ] Rollback Protection
- [ ] YMODEM Support

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
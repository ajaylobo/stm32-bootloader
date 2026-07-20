# Bootloader Design

## Objective

The objective of this project is to understand and implement a production-style STM32 bootloader capable of safely transferring execution to an independently linked application.

---

## Design Goals

- Independent bootloader and application
- Application image validation
- Reliable bootloader-to-application handoff
- Cortex-M compliant startup sequence
- Minimal application dependencies
- Scalable architecture for future firmware updates

---

## Boot Sequence

Power On

↓

Reset_Handler

↓

SystemInit()

↓

Bootloader main()

↓

Validate Application

↓

Cleanup MCU State

↓

Relocate VTOR

↓

Switch MSP

↓

Jump to Application

---

## Design Decisions

### Separate linker scripts

The application is linked to 0x08010000 to reserve flash space for the bootloader.

### Application Validation

The bootloader validates:

- Initial MSP
- Reset Handler

before transferring execution.

### Interrupt Cleanup

Before the jump:

- Disable interrupts
- Clear pending interrupts
- Stop SysTick

This prevents interrupt state leakage into the application.

### Memory Barriers

DSB and ISB are used after updating VTOR to guarantee correct ordering of memory operations and instruction execution before transferring control.

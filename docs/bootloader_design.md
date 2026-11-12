# Bootloader Design

## Objective

The objective of this project is to understand and implement an STM32 bootloader that transfers execution to an independently linked application and can receive and install an application image over UART.

---

## Design Goals

- Independent bootloader and application
- Application image validation
- Reliable bootloader-to-application handoff
- UART-based firmware transfer and flash programming
- Packet-level error detection and ACK/NACK responses
- Cortex-M compliant startup sequence
- Separation between bootloader and application images

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

Wait up to 3 seconds for START_UPDATE over USART2

↓

Update requested?

├── Yes → erase application sectors, receive packets, validate image, reset

└── No → validate existing application and hand off when valid

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

### UART Firmware Update

USART2 is configured for 115200 baud, 8 data bits, no parity, and 1 stop bit, using PA2 for TX and PA3 for RX. A receive interrupt places bytes in a ring buffer; the main loop parses framed packets. Each update packet is acknowledged before the host sends the next one.

`START_UPDATE` supplies the firmware size. The updater erases sectors 4 and 5, writes incoming data to the application base at `0x08010000`, and checks that the image's initial stack pointer and reset handler fall in expected address ranges. A successful `END_UPDATE` is acknowledged and followed by a system reset.

The current application image limit is 192 KiB, matching the erased sectors and the application linker script. The frame CRC detects packet corruption; there is no whole-image CRC, authentication, version check, or rollback mechanism. The protocol guide documents the frame layout and additional behavior and limitations.

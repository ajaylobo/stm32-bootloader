# Memory Layout

## Overview

The STM32F446RE contains:

- 512 KB Internal Flash
- 128 KB SRAM

The flash memory is divided into two independent regions.

```
Flash Memory

0x08000000
┌──────────────────────────────┐
│                              │
│        Bootloader            │
│          64 KB               │
│                              │
└──────────────────────────────┘
0x08010000
┌──────────────────────────────┐
│                              │
│        Application           │
│          448 KB              │
│                              │
└──────────────────────────────┘
0x08080000
```

---

## SRAM Layout

```
0x20020000
┌──────────────────────────────┐
│          Stack               │
├──────────────────────────────┤
│           Heap               │
├──────────────────────────────┤
│           .bss               │
├──────────────────────────────┤
│           .data              │
└──────────────────────────────┘
0x20000000
```

---

## Flash Sections

| Section | Description |
|----------|-------------|
| .isr_vector | Interrupt Vector Table |
| .text | Executable Code |
| .rodata | Read-only Data |
| .data | Initialized Variables (stored in Flash, copied to RAM) |
| .bss | Zero Initialized Variables |

---

## Why relocate the Application?

The bootloader occupies the beginning of flash.

Therefore, the application must be linked to start at:

```
0x08010000
```

instead of

```
0x08000000
```

The linker script informs the linker where the application image should be placed in flash.

---

## Bootloader Memory Reservation

| Region | Start | Size |
|---------|--------|------|
| Bootloader | 0x08000000 | 64 KB |
| Application | 0x08010000 | 448 KB |
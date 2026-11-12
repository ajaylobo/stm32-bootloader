# Memory Layout

## Overview

The STM32F446RE contains:

- 512 KB Internal Flash
- 128 KB SRAM

The current layout reserves the first 64 KB for the bootloader and uses the next 192 KB for the application. Although the MCU has more flash, the application linker script and updater currently stop at `0x08040000`.

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
│          192 KB              │
│                              │
└──────────────────────────────┘
0x08040000
┌──────────────────────────────┐
│ Not used by current updater  │
│          256 KB              │
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
| Application image | 0x08010000 | 192 KB |
| Remaining flash | 0x08040000 | 256 KB (not used by the current updater) |

The updater erases sectors 4 and 5, which cover `0x08010000`–`0x0803FFFF`. The application linker script has the same 192 KB limit. Do not assume the remaining physical flash can hold an update without changing both the linker layout and erase logic.
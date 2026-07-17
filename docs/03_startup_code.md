# Startup Code

## Overview

The startup code is the first firmware executed after Reset_Handler.

Its primary responsibility is to prepare the execution environment before entering main().

---

## Startup Sequence

```
Reset

↓

Reset_Handler

↓

Copy .data

↓

Clear .bss

↓

SystemInit()

↓

__libc_init_array()

↓

main()
```

---

## Responsibilities

### Copy Initialized Variables

Variables initialized by the programmer are stored in flash.

Example

```c
int speed = 100;
```

The startup code copies these values into RAM.

---

### Clear .bss

Variables without an explicit initial value are initialized to zero.

Example

```c
int counter;
```

The startup code clears the .bss section.

---

### SystemInit()

SystemInit() configures:

- Clock system
- FPU
- Vector Table (optional)

before entering main().

---

## Why is startup code important?

Without startup code:

- Global variables would contain incorrect values.
- Static variables would not be initialized.
- Clock configuration would be incorrect.
- The C runtime environment would not exist.
# Vector Table

## Overview

Every Cortex-M microcontroller begins execution by reading the Vector Table located in flash memory.

The Vector Table contains:

- Initial Main Stack Pointer (MSP)
- Reset Handler
- Exception Handlers
- Peripheral Interrupt Handlers

```
Application Vector Table

0x08010000
+-------------------------------+
| Initial MSP                   |
+-------------------------------+
| Reset_Handler                 |
+-------------------------------+
| NMI_Handler                   |
+-------------------------------+
| HardFault_Handler             |
+-------------------------------+
| MemManage_Handler             |
+-------------------------------+
| BusFault_Handler              |
+-------------------------------+
| UsageFault_Handler            |
+-------------------------------+
| ...                           |
+-------------------------------+
```

---

## Boot Sequence

After reset the CPU performs the following steps automatically:

1. Load MSP from address 0x08000000.
2. Load Reset_Handler from address 0x08000004.
3. Jump to Reset_Handler.

No firmware code is executed before these operations.

---

## Why relocate VTOR?

After the bootloader decides to execute the application, interrupts must use the application's vector table.

Therefore:

```c
SCB->VTOR = APP_ADDRESS;
```

changes the interrupt vector table location to:

```
0x08010000
```

Without updating VTOR, every interrupt would continue using the bootloader's interrupt handlers.
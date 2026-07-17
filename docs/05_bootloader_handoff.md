# Bootloader Handoff

## Overview

Once a valid application is detected, the bootloader transfers execution to the application.

This process is commonly referred to as the "Bootloader Handoff".

---

## Steps

1. Disable interrupts.
2. Stop SysTick.
3. Deinitialize HAL.
4. Relocate VTOR.
5. Load application's MSP.
6. Enable interrupts.
7. Jump to application's Reset_Handler.

---

## Implementation

```c
SCB->VTOR = APP_ADDRESS;

__set_MSP(app_msp);

__enable_irq();

app_entry();
```

---

## Why is MSP switched?

The application has its own stack.

Before executing application code, the CPU must use the application's stack rather than the bootloader's stack.

---

## Why relocate VTOR?

Interrupts occurring after the jump must invoke the application's interrupt handlers.

Without relocating VTOR, interrupts would continue to execute the bootloader's handlers.

---

## Debugging Notes

During development, the application initially failed to execute correctly after the jump.

The issue was diagnosed by inspecting:

- MSP
- VTOR
- PC
- PRIMASK

The root cause was that interrupts remained globally disabled (`PRIMASK = 1`) after the bootloader disabled them. As a result, the SysTick interrupt never executed, preventing `HAL_GetTick()` from updating the system tick and causing `HAL_Delay()` to block indefinitely.

Enabling interrupts before transferring control resolved the issue and allowed the application to start successfully.

---

## Current Status

✅ Successful Bootloader → Application Handoff
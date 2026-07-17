# Debugging Journal

This document records the debugging process followed during the development of the STM32 Bootloader. The objective is to document not only the final implementation but also the engineering approach used to identify and resolve issues encountered during development.

---

# Debug Session 1

## Problem

The bootloader was successfully validating the application, but the application did not start executing after the jump.

Observed behaviour:

- Bootloader LED blinked correctly when no application was present.
- After flashing the application, the bootloader LED remained continuously ON.
- The application LED never blinked.

---

## Initial Hypothesis

Possible causes considered:

- Incorrect linker script
- Incorrect application start address
- Invalid vector table
- Incorrect MSP value
- Incorrect Reset_Handler address
- VTOR not relocated
- Fault during bootloader handoff

---

# Investigation

## Step 1

Verified application validation.

Result:

```
Application detected successfully.
```

Validation logic was functioning correctly.

---

## Step 2

Verified application linker script.

Application Flash Origin:

```
0x08010000
```

Confirmed that the application was linked to the relocated address.

---

## Step 3

Verified Reset_Handler address.

Map file:

```
Reset_Handler = 0x080107F0
```

Application image appeared valid.

---

## Step 4

Stepped through JumpToApplication() using the debugger.

Verified:

```
MSP
VTOR
Program Counter
```

Observed:

```
MSP = 0x20020000
VTOR = 0x08010000
```

Both values were correct before executing the jump.

---

## Step 5

Single-stepped through:

```c
app_entry();
```

The debugger lost source synchronization.

Initially this appeared to indicate that the jump had failed.

Further investigation was required.

---

# Debug Session 2

## Program Counter Investigation

The Program Counter (PC) was inspected after the jump.

Observed value:

```
PC = 0x08010986
```

Cross-referencing the application map file showed that this address belonged to the application's HAL library.

This confirmed that execution had successfully transferred from the bootloader to the application.

The problem was therefore occurring after the jump rather than during the jump.

---

# Debug Session 3

## HAL Delay Investigation

The Program Counter remained inside:

```
HAL_GetTick()
```

This suggested that the application was blocked waiting for the system tick.

The following hypothesis was formed:

```
HAL_Delay()

↓

HAL_GetTick()

↓

SysTick interrupt not executing
```

---

# Debug Session 4

## PRIMASK Inspection

The Cortex-M register PRIMASK was inspected.

Observed:

```
PRIMASK = 1
```

Meaning:

```
Global Interrupts Disabled
```

The bootloader correctly disabled interrupts before the handoff:

```c
__disable_irq();
```

However, interrupts were never re-enabled before transferring execution.

As a result:

- SysTick interrupt never executed
- uwTick never incremented
- HAL_Delay() never returned

---

# Resolution

Before transferring execution to the application:

```c
__enable_irq();
```

was added.

The bootloader handoff sequence became:

```c
SCB->VTOR = APP_ADDRESS;

__set_MSP(app_msp);

__enable_irq();

app_entry();
```

---

# Result

The application started successfully.

Observed behaviour:

```
Power On

↓

Bootloader

↓

Validate Application

↓

JumpToApplication()

↓

Application Reset_Handler

↓

Application main()

↓

LED Blink
```

The bootloader handoff was successfully completed.

---

# Lessons Learned

This debugging session reinforced several important Cortex-M concepts:

- The Program Counter is one of the most valuable debugging tools.
- Successful application validation does not guarantee successful execution.
- MSP and VTOR should always be verified before the jump.
- PRIMASK directly affects SysTick and HAL timing functions.
- HAL_Delay() depends on SysTick interrupts updating the global tick counter.
- Debugging should be driven by evidence rather than assumptions.

---

# Future Debugging Topics

As the bootloader evolves, this journal will include investigations related to:

- UART firmware update
- Flash erase/program failures
- CRC verification
- Metadata validation
- Interrupt cleanup
- Fault handling
- Bootloader recovery mechanisms

---

# Version History

| Version | Description |
|----------|-------------|
| v0.1.0 | First successful Bootloader → Application handoff |
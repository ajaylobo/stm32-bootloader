# Application Validation

## Objective

Before transferring execution to the application, the bootloader must determine whether a valid firmware image exists.

Jumping to an invalid application can cause undefined behaviour.

---

## Validation Strategy

The bootloader verifies:

- Initial MSP is inside SRAM.
- Reset Handler is inside Flash.

Example

```c
bool IsApplicationValid(void)
{
    ...
}
```

---

## MSP Validation

The first word of the vector table contains the application's initial MSP.

For STM32F446RE:

```
0x20000000 <= MSP <= 0x20020000
```

---

## Reset Handler Validation

The second word contains the Reset_Handler address.

The current implementation requires it to be at or above the application start and no higher than the MCU's physical flash end:

```
0x08010001 <= Reset_Handler <= 0x0807FFFF
```

This upper bound is wider than the 192 KiB application region in the linker script. The check is a basic vector sanity check; it does not prove that the entire image is valid or that the reset handler lies within the bytes received during an update.

---

## Future Improvements

The current implementation checks only the initial MSP and Reset_Handler values. Update packet CRC and received-size checks do not replace whole-image validation.

Future versions will additionally verify:

- Whole-image CRC
- Firmware metadata and version
- Hardware compatibility
- Digital signature
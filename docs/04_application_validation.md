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

It must lie inside the application's flash region.

```
0x08010000 <= Reset_Handler <= 0x08080000
```

---

## Future Improvements

Current implementation validates only the vector table.

Future versions will additionally verify:

- Firmware CRC
- Firmware Version
- Firmware Size
- Digital Signature
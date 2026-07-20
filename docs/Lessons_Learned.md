# Lessons Learned

## Bootloader Development

- Cortex-M automatically loads MSP before executing Reset_Handler.
- Application relocation requires linker script modification.
- VTOR must point to the application's vector table before enabling interrupts.
- Pending interrupts must be cleared before application handoff.
- MSP must be updated before executing application code.
- Bootloader should only transfer control from Thread Mode.
- DSB and ISB eliminate timing-dependent behaviour after system register updates.

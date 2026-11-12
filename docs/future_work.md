# Future Work

The current implementation supports application handoff and a basic UART firmware update path. The current updater is limited to a 192 KiB image and provides packet-level CRC only.

Potential future improvements:

## Firmware Validation and Metadata

- Add whole-image CRC verification.
- Add firmware metadata, version and hardware compatibility checks.

## Update Robustness

- Add packet retries and recovery from interrupted transfers.
- Add a timeout after an update starts.
- Add rollback or dual-image support.
- Consider watchdog recovery and authenticated/secure boot.
- Extend the updater and linker layout together if support for firmware larger than 192 KiB is needed.
- Consider YMODEM or another standard transfer protocol.

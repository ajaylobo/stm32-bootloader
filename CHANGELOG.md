# Changelog

## Unreleased

### Added
- USART2 firmware transfer at 115200 baud using framed packets and packet-level CRC
- ACK/NACK responses for update commands
- Flash erase/program update flow for application images up to 192 KiB
- Python serial programmer in `Programmer/stm_programmer.py`
- UART protocol and usage guide

### Documentation
- Updated the boot flow, memory map, implementation status, and roadmap to describe UART firmware updates and current limits

## v0.1.0

### Added
- Bootloader project
- Application project
- Linker script relocation
- Application validation
- Main Stack Pointer (MSP) switching
- Vector Table (VTOR) relocation
- Successful bootloader-to-application handoff

### Improved
- Added NVIC interrupt cleanup before application handoff
- Cleared pending interrupts before jumping to the application
- Added Data Synchronization Barrier (DSB) and Instruction Synchronization Barrier (ISB) before application handoff
- Improved bootloader transition reliability

### Fixed
- Resolved interrupt masking issue after bootloader handoff

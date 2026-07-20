# Changelog

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

RESET
  │
  ▼
Bootloader Start
  │
  ▼
Wait up to 3 seconds for START_UPDATE on USART2
  │
  ├── Update requested
  │     │
  │     ▼
  │   Erase application sectors 4 and 5
  │     │
  │     ▼
  │   Receive DATA packets and send ACK/NACK responses
  │     │
  │     ▼
  │   END_UPDATE: check byte count and application vectors
  │     │
  │     ├── Valid → ACK and system reset
  │     └── Invalid/error → remain in update mode
  │
  └── No update request
        │
        ▼
      Validate current application
        │
        ├── Invalid → Stay in bootloader
        │
        └── Valid
              │
              ▼
        Cleanup MCU state
              │
              ▼
        Disable interrupts, clear NVIC state, stop SysTick
              │
              ▼
        HAL/RCC DeInit, relocate VTOR, set MSP
              │
              ▼
        JumpToApplication()
              │
              ▼
        Application Reset_Handler

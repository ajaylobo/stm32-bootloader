                   RESET

                     │

                     ▼

             Bootloader Start

                     │

                     ▼

         Validate Application Image

             │               │

             │               │

         Invalid          Valid

             │               │

             ▼               ▼

     Stay in Bootloader   Cleanup MCU

                             │

                             ▼

                    Disable Interrupts

                             │

                             ▼

                     Clear NVIC State

                             │

                             ▼

                       Stop SysTick

                             │

                             ▼

                     HAL/RCC DeInit

                             │

                             ▼

                       Relocate VTOR

                             │

                             ▼

                        Set MSP

                             │

                             ▼

                    JumpToApplication()

                             │

                             ▼

                 Application Reset_Handler

# UART Firmware Update

This guide describes the UART firmware update currently implemented for the STM32F446RE bootloader.

## Requirements and connections

- STM32F446RE Nucleo board with the bootloader programmed at `0x08000000`.
- Application built for the relocated flash address `0x08010000`.
- A serial connection to USART2: PA2 is TX and PA3 is RX.
- A 3.3 V-compatible UART connection with TX/RX crossed and a shared ground. The Nucleo ST-LINK virtual COM connection can be used when it is routed to USART2.
- Python 3 and PySerial.

USART2 is configured for 115200 baud, 8 data bits, no parity, and 1 stop bit (8-N-1), with no hardware flow control. The programmer defaults to `COM4`; change `PORT` in `Programmer/stm_programmer.py` to match the port on your computer.

## Transfer workflow

1. Build the application with the application linker script, whose flash origin is `0x08010000`, and use the generated raw `.bin` file.
2. Reset the board. The bootloader waits up to 3 seconds for a valid update request. If no update starts during this window, it validates and boots the current application when possible.
3. Install PySerial if needed and run the programmer:

   ```powershell
   python -m pip install pyserial
   python Programmer\stm_programmer.py path\to\application.bin
   ```

4. The programmer sends `START_UPDATE` with the image length, then sends the image in packets of up to 128 bytes. It waits for an ACK after each packet before sending the next one.
5. The programmer sends `END_UPDATE`. The bootloader verifies the received byte count and basic application vector values. If successful, it sends an ACK and resets; the new application should then start.

The programmer reports an error if it receives a NACK, malformed response, or no complete response within its 10-second serial read timeout. It does not retry packets.

## Packet format

All multi-byte integer fields are little-endian. The maximum payload is 128 bytes.

| Field | Size | Description |
|-------|------|-------------|
| SOF | 1 byte | Start marker, `0xAA` |
| Protocol version | 1 byte | `0x01` |
| Command | 1 byte | Request or response command |
| Payload length | 2 bytes | Unsigned length, low byte first |
| Payload | 0–128 bytes | Command-specific data |
| CRC | 2 bytes | CRC low byte first |

The CRC is CRC-16/CCITT with initial value `0xFFFF` and polynomial `0x1021`. It covers the protocol version, command, both length bytes, and payload; it does not cover SOF. There is no final XOR.

### Request commands

| Command | Value | Payload |
|---------|-------|---------|
| `START_UPDATE` | `0x01` | Firmware size as a 4-byte unsigned little-endian integer |
| `DATA` | `0x02` | 1–128 bytes of firmware data |
| `END_UPDATE` | `0x03` | Empty |
| `GET_VERSION` | `0x04` | Defined but not implemented; no version response is currently generated |

### ACK/NACK response

The response command is `ACK` (`0x05`) or `NACK` (`0x06`). Its two-byte payload contains the original request command followed by a status byte (`0` means success). The response uses the same packet format and CRC calculation. The host should match the original command and check both the response command and status.

## Flash and validation limits

- Application start: `0x08010000`.
- Maximum image size: 192 KiB (`196608` bytes).
- The updater erases flash sectors 4 and 5 before receiving data. These sectors cover the 192 KiB application region through `0x0803FFFF`.
- The application linker script is also limited to 192 KiB. The remaining physical flash is not currently erased or made available to this updater.
- Data is programmed to flash in 32-bit words. A final partial word is padded with `0xFF`.
- At completion, the bootloader checks the received byte count, initial stack pointer range, and reset-handler address range. The packet CRC protects individual frames against accidental corruption; the bootloader does not verify a whole-image CRC or authenticate the image.

## Operational limitations

- The 3-second window applies only before an update is started. There is no transfer timeout once update mode has begun.
- The protocol is stop-and-wait, but the programmer has no retransmission or resume support.
- An interrupted update can leave the previous application erased or incomplete. No backup image or rollback mechanism is implemented.
- `GET_VERSION`, firmware metadata, cryptographic verification, and whole-image CRC validation are not implemented.

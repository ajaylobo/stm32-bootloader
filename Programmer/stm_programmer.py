
import serial
import struct
import sys
import os

SOF = 0xAA
PROTOCOL_VERSION = 0x01

BOOT_CMD_START_UPDATE = 0x01
BOOT_CMD_DATA = 0x02
BOOT_CMD_END_UPDATE = 0x03
BOOT_CMD_GET_VERSION = 0x04
BOOT_CMD_ACK = 0x05
BOOT_CMD_NACK = 0x06

MAX_PAYLOAD_SIZE = 128
MAX_FIRMWARE_SIZE = 192 * 1024

INITIAL_CRC = 0xFFFF
POLYNOMIAL = 0x1021

PORT = "COM4"
BAUDRATE = 115200


def calculate_crc(crc, byte):
    crc ^= byte << 8

    for _ in range(8):
        msb = (crc & 0x8000) != 0
        crc = (crc << 1) & 0xFFFF

        if msb:
            crc = (crc ^ POLYNOMIAL) & 0xFFFF

    return crc


def build_packet(command, payload):

    length = len(payload)

    packet = bytearray()
    packet.append(SOF)
    packet.append(PROTOCOL_VERSION)
    packet.append(command)
    packet.append(length & 0xFF)
    packet.append((length >> 8) & 0xFF)
    packet.extend(payload)

    crc = INITIAL_CRC

    for byte in packet[1:]:
        crc = calculate_crc(crc, byte)

    packet.append(crc & 0xFF)
    packet.append((crc >> 8) & 0xFF)

    return packet


def send_packet(packet):

    try:
        bytes_written = ser.write(packet)
        ser.flush()
        return bytes_written == len(packet)

    except (serial.SerialException, serial.SerialTimeoutException):
        return False


def receive_response():

    response = ser.read(9)

    print("Received length:", len(response))
    print("Received data:", response.hex())

    if len(response) != 9:
        print("ERROR: Incomplete response")
        return None

    if response[0] != SOF:
        print("ERROR: Invalid SOF")
        return None

    if response[1] != PROTOCOL_VERSION:
        print("ERROR: Invalid protocol version")
        return None

    if response[2] not in (BOOT_CMD_ACK, BOOT_CMD_NACK):
        print("ERROR: Invalid response command")
        return None

    if response[3] != 2 or response[4] != 0:
        print("ERROR: Invalid response length")
        return None

    # Calculate response CRC
    crc = INITIAL_CRC

    for byte in response[1:7]:
        crc = calculate_crc(crc, byte)

    received_crc = response[7] | (response[8] << 8)

    if crc != received_crc:
        print("ERROR: Response CRC mismatch")
        return None

    return response


def response_is_ack(response, original_command):

    if response is None:
        return False

    if response[5] != original_command:
        print("ERROR: Response command mismatch")
        return False

    if response[2] == BOOT_CMD_NACK:
        print("NACK received. Status:", response[6])
        return False

    if response[2] != BOOT_CMD_ACK or response[6] != 0:
        print("ERROR: Invalid ACK status")
        return False

    return True


def send_start_update(firmware_size):

    payload = struct.pack("<I", firmware_size)
    packet = build_packet(BOOT_CMD_START_UPDATE, payload)

    if not send_packet(packet):
        print("Failed to transmit START_UPDATE")
        return False

    response = receive_response()

    return response_is_ack(response, BOOT_CMD_START_UPDATE)


def send_data(payload):

    if not payload or len(payload) > MAX_PAYLOAD_SIZE:
        print("ERROR: Invalid DATA payload size")
        return False

    packet = build_packet(BOOT_CMD_DATA, payload)

    if not send_packet(packet):
        print("Failed to transmit DATA")
        return False

    response = receive_response()

    return response_is_ack(response, BOOT_CMD_DATA)


def send_end_data():

    packet = build_packet(BOOT_CMD_END_UPDATE, b"")

    if not send_packet(packet):
        print("Failed to transmit END_UPDATE")
        return False

    response = receive_response()

    return response_is_ack(response, BOOT_CMD_END_UPDATE)


def main():

    global ser

    if len(sys.argv) != 2:
        print("Usage: python3 programmer.py <firmware.bin>")
        return

    firmware_path = sys.argv[1]

    if not os.path.isfile(firmware_path):
        print("ERROR: Firmware file not found")
        return

    with open(firmware_path, "rb") as f:
        firmware_data = f.read()

    firmware_size = len(firmware_data)

    if firmware_size == 0 or firmware_size > MAX_FIRMWARE_SIZE:
        print("ERROR: Firmware size invalid:", firmware_size)
        return

    print("\nFirmware:", firmware_path)
    print("Firmware size:", firmware_size, "bytes")


    try:
        with serial.Serial(
            port=PORT,
            baudrate=BAUDRATE,
            timeout=10,
            write_timeout=3
        ) as ser:

            print("\nSerial port opened:", ser.is_open)

            ser.reset_input_buffer()

            # STEP 1: START UPDATE
            print("\n--- START UPDATE ---")

            success = send_start_update(firmware_size)

            if not success:
                print("START_UPDATE FAILED")
                return

            print("START_UPDATE ACK received")

            # STEP 2: SEND FIRST 8 BYTES
            print("\n--- DATA TRANSFER ---")

            print("Sending fimware data over UART")

            for offset in range(0, firmware_size, MAX_PAYLOAD_SIZE):

                payload = firmware_data[offset : offset + MAX_PAYLOAD_SIZE]

                success = send_data(payload)

                if not success:
                    print("DATA TRANSFER FAILED")
                    return

                progress = ((offset + len(payload)) / firmware_size) * 100
                print(f"Transferred: {offset + len(payload)}/{firmware_size} bytes ({progress:.1f}%)")

            print("DATA ACK received")

            success = send_end_data()
            if not success:
                print("firmware update failed")
                return
            print("firmware update successfull")



    except serial.SerialException as error:
        print("Serial error:", error)


if __name__ == "__main__":
    main()

//
// Created by Iman on 8/21/2026.
//

#include "protocol.h"

namespace RSProtocol {

Protocol::Protocol(Stream &stream)
    : stream_(stream)
{
}

void Protocol::put16(uint8_t *buffer, uint16_t value)
{
    // Big endian: |HB|LB|
    buffer[0] = (value >> 8) & 0xFF;
    buffer[1] = value & 0xFF;
}

uint16_t Protocol::get16(const uint8_t *buffer)
{
    return (static_cast<uint16_t>(buffer[0]) << 8) |
           static_cast<uint16_t>(buffer[1]);
}

uint16_t Protocol::crc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            }
            else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

void Protocol::send(const SlavePacket &packet)
{
    // Raw 19-byte payload only, no SYNC/TYPE/LEN/CRC by design:
    // PAYLOAD(19: FC...23).
    uint8_t payload[PAYLOAD_SIZE];
    uint8_t index = 0;

    payload[index++] = packet.header;

    put16(&payload[index], packet.ditherAmp);
    index += 2;

    put16(&payload[index], packet.ditherFreq);
    index += 2;

    put16(&payload[index], packet.biasPoint);
    index += 2;

    put16(&payload[index], packet.ditherOn);
    index += 2;

    payload[index++] = packet.calibrationRequest;

    for (uint8_t i = 0; i < 4; ++i) {
        put16(&payload[index], packet.rst[i]);
        index += 2;
    }

    payload[index++] = packet.footer;

    stream_.write(payload, PAYLOAD_SIZE);
}

bool Protocol::poll(MasterPacket &packet)
{
    while (stream_.available()) {

        const int value = stream_.read();

        if (value < 0) {
            break;
        }

        if (processByte(static_cast<uint8_t>(value))) {
            const bool ok = parseMaster(rxBuffer_, PAYLOAD_SIZE, packet);
            resetParser();
            if (ok) {
                return true;
            }
        }
    }

    return false;
}

bool Protocol::processByte(uint8_t byte)
{
    // Sliding 19-byte window: master and slave share the same
    // FC...23 framing, so accept any window starting with 0xFC
    // and ending with 0x23. Own-TX echo is drained by RSPort::send,
    // so it never reaches this parser.
    for (uint8_t i = 0; i < PAYLOAD_SIZE - 1; i++) {
        rxBuffer_[i] = rxBuffer_[i + 1];
    }
    rxBuffer_[PAYLOAD_SIZE - 1] = byte;

    if (rxBuffer_[0] == 0xFC && rxBuffer_[PAYLOAD_SIZE - 1] == 0x23) {
        return true;
    }

    return false;
}

bool Protocol::parseMaster(const uint8_t *data, uint8_t length, MasterPacket &packet)
{
    constexpr uint8_t EXPECTED_SIZE = 20;

    if (length != EXPECTED_SIZE) {
        return false;
    }

    uint8_t index = 0;

    packet.header = data[index++];

    packet.biasVoltage = get16(&data[index]);
    index += 2;

    packet.inputPower = get16(&data[index]);
    index += 2;

    packet.outputPower = get16(&data[index]);
    index += 2;

    packet.temperature = get16(&data[index]);
    index += 2;

    packet.calibrationDone = data[index++];

    // byte 10 is an extra field not documented in the spec — skip it
    index += 1;

    packet.rsv[0] = get16(&data[index]);
    index += 2;

    packet.rsv[1] = get16(&data[index]);
    index += 2;

    packet.rsv[2] = get16(&data[index]);
    index += 2;

    packet.rsv[3] = get16(&data[index]);
    index += 2;

    packet.footer = data[index++];

    return true;
}

void Protocol::resetParser()
{
    // Sliding-window parser holds no per-frame state.
}

}

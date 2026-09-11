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
    uint8_t frame[PAYLOAD_SIZE];
    uint8_t index = 0;

    frame[index++] = packet.header;

    put16(&frame[index], packet.ditherAmp);
    index += 2;

    put16(&frame[index], packet.ditherFreq);
    index += 2;

    put16(&frame[index], packet.biasPoint);
    index += 2;

    put16(&frame[index], packet.ditherOn);
    index += 2;

    put16(&frame[index], packet.calibrationRequest);
    index += 2;

    for (uint8_t i = 0; i < 3; ++i) {
        put16(&frame[index], packet.rst[i]);
        index += 2;
    }

    frame[index++] = packet.footer;

    const uint16_t crc = crc16(&frame[1], 2 + PAYLOAD_SIZE);

    stream_.write(frame, index);
}

bool Protocol::poll(MasterPacket &packet)
{
    while (stream_.available()) {

        const int value = stream_.read();

        if (value < 0) {
            break;
        }

        if (processByte(static_cast<uint8_t>(value))) {
            const bool ok = parseMaster(rxBuffer_, 20, packet);
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
    switch (state_) {

        case WAIT_HEADER:
            if (byte == 0xFC) {
                rxBuffer_[0] = byte;
                rxIndex_ = 1;
                state_ = READ_PAYLOAD;
            }
            break;

        case READ_PAYLOAD:
            rxBuffer_[rxIndex_++] = byte;

            if (rxIndex_ >= 20) {
                return true;
            }
            break;
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

    packet.calibrationDone = get16(&data[index]);
    index += 2;

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
    state_ = WAIT_HEADER;
    rxIndex_ = 0;
}

}

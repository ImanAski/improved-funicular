//
// Created by Iman on 8/21/2026.
//

#include "protocol.h"

#if RS485_DEBUG
static const char *_rs_type(uint8_t type) {
    switch (type) {
        case 0x01: return "MASTER";
        case 0x02: return "SLAVE";
        default:   return "UNKNOWN";
    }
}

static void _rs_hex(const uint8_t *data, size_t len, const char *label) {
    Serial.printf("[RS485] %s: ", label);
    for (size_t i = 0; i < len; ++i) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();
}
#endif

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
    uint8_t frame[3 + PAYLOAD_SIZE + 2];
    uint8_t index = 0;

    frame[index++] = SYNC;
    frame[index++] = TYPE_SLAVE;
    frame[index++] = PAYLOAD_SIZE;

    frame[index++] = packet.header;

    put16(&frame[index], packet.ditherAmp);
    index += 2;

    put16(&frame[index], packet.ditherFreq);
    index += 2;

    put16(&frame[index], packet.biasPoint);
    index += 2;

    put16(&frame[index], packet.ditherOn);
    index += 2;

    frame[index++] = packet.calibrationRequest;

    for (uint8_t i = 0; i < 4; ++i) {
        put16(&frame[index], packet.rst[i]);
        index += 2;
    }

    frame[index++] = packet.footer;

    const uint16_t crc = crc16(&frame[1], 2 + PAYLOAD_SIZE);

    put16(&frame[index], crc);
    index += 2;

#if RS485_DEBUG
    Serial.printf("[RS485] TX TYPE=%s len=%u crc=0x%04X\n", _rs_type(TYPE_SLAVE), PAYLOAD_SIZE, crc);
    _rs_hex(frame, index, "TX FRAME");
    Serial.printf("[RS485] TX payload: ditherAmp=%u ditherFreq=%u biasPoint=%u ditherOn=%u calReq=%u\n",
        packet.ditherAmp, packet.ditherFreq, packet.biasPoint, packet.ditherOn, packet.calibrationRequest);
#endif

    stream_.write(frame, index);
}

bool Protocol::poll(MasterPacket &packet)
{
    while (stream_.available()) {
        Serial.println("ttest");

        const int value = stream_.read();

        if (value < 0) {
            break;
        }

        if (processByte(static_cast<uint8_t>(value))) {
            const bool ok = parseMaster(&rxBuffer_[0], 19, packet);
            resetParser();
            if (ok) {
#if RS485_DEBUG
                Serial.printf("[RS485] RX TYPE=MASTER biasV=%u inputP=%u outputP=%u temp=%u calDone=%u\n",
                    packet.biasVoltage, packet.inputPower, packet.outputPower, packet.temperature, packet.calibrationDone);
#endif
                return true;
            }
        }
    }

    return false;
}

bool Protocol::processByte(uint8_t byte)
{
    switch (state_) {

        case WAIT_SYNC:
            if (byte == SYNC) {
#if RS485_DEBUG
                Serial.printf("[RS485] RX byte=0x%02X state=SYNC\n", byte);
#endif
                state_ = READ_TYPE;
            }
            break;

        case READ_TYPE:
#if RS485_DEBUG
            Serial.printf("[RS485] RX byte=0x%02X state=TYPE=%s\n", byte, _rs_type(byte));
#endif
            state_ = READ_LEN;
            break;

        case READ_LEN:
#if RS485_DEBUG
            Serial.printf("[RS485] RX byte=0x%02X state=LEN=%u\n", byte, byte);
#endif
            state_ = READ_PAYLOAD;
            rxIndex_ = 0;
            break;

        case READ_PAYLOAD:
            rxBuffer_[rxIndex_++] = byte;
#if RS485_DEBUG
            if (rxIndex_ == 1) {
                Serial.printf("[RS485] RX payload start (19 bytes expected)\n");
            }
            if (rxIndex_ == 19) {
                _rs_hex(rxBuffer_, 19, "RX PAYLOAD");
            }
#endif
            if (rxIndex_ >= 19) {
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

    packet.calibrationDone = data[index++];

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
#if RS485_DEBUG
    Serial.printf("[RS485] Parser reset\n");
#endif
    state_ = WAIT_SYNC;
    rxIndex_ = 0;
}

}

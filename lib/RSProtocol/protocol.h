//
// Created by Iman on 8/21/2026.
//

#ifndef RS_PROTOCOL_H
#define RS_PROTOCOL_H

#include <Arduino.h>

namespace RSProtocol {

struct MasterPacket {
    uint8_t header;
    uint16_t biasVoltage;
    uint16_t inputPower;
    uint16_t outputPower;
    uint16_t temperature;
    uint8_t calibrationDone;
    uint16_t rsv[4];
    uint8_t footer;
};

struct SlavePacket {
    uint8_t header;
    uint16_t ditherAmp;
    uint16_t ditherFreq;
    uint16_t biasPoint;
    uint16_t ditherOn;
    uint8_t calibrationRequest;
    uint16_t rst[4];
    uint8_t footer;
};

class Protocol {
public:
    static constexpr uint8_t SYNC = 0xAA;
    static constexpr uint8_t TYPE_MASTER = 0x01;
    static constexpr uint8_t TYPE_SLAVE = 0x02;

    explicit Protocol(Stream &stream);

    void send(const SlavePacket &packet);

    bool poll(MasterPacket &packet);

private:
    static constexpr uint8_t PAYLOAD_SIZE = 19;
    static constexpr uint8_t MASTER_PAYLOAD_SIZE = 20;
    static constexpr size_t RX_BUFFER_SIZE = 64;

    bool processByte(uint8_t byte);
    void resetParser();

    static bool parseMaster(const uint8_t *data, uint8_t length, MasterPacket &packet);
    static uint16_t crc16(const uint8_t *data, size_t length);
    static void put16(uint8_t *buffer, uint16_t value);
    static uint16_t get16(const uint8_t *buffer);

    Stream &stream_;

    uint8_t rxBuffer_[RX_BUFFER_SIZE];
    uint8_t rxIndex_ = 0;

    enum State : uint8_t {
        WAIT_HEADER,
        READ_PAYLOAD,
    };

    State state_ = WAIT_HEADER;
};

}

#endif

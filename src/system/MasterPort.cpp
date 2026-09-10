//
// Created by Iman on 9/10/2026.
//

#include "MasterPort.h"

#include <Arduino.h>

namespace MasterPort {

namespace {
    constexpr uint8_t kFrameSize = 20;
    constexpr uint8_t kFrameHeader = 0xFC;
    constexpr uint8_t kFrameFooter = 0x23;

    uint8_t s_rxBuffer[kFrameSize];
    SoftwareSerial *s_serial = nullptr;
    uint8_t s_dirPin = 0;

    uint16_t get16(const uint8_t *buffer) {
        return (static_cast<uint16_t>(buffer[0]) << 8) |
               static_cast<uint16_t>(buffer[1]);
    }

    void put16(uint8_t *buffer, uint16_t value) {
        buffer[0] = (value >> 8) & 0xFF;
        buffer[1] = value & 0xFF;
    }

    void receiveMode() {
        if (s_dirPin) {
            digitalWrite(s_dirPin, LOW);
        }
    }

    void transmitMode() {
        if (s_dirPin) {
            digitalWrite(s_dirPin, HIGH);
        }
    }
}

void init(uint8_t txPin, uint8_t rxPin, uint32_t baud) {
    SoftwareSerial *ss = new SoftwareSerial(txPin, rxPin);
    ss->begin(baud);
    s_serial = ss;
}

void begin(uint8_t directionPin, uint32_t baud) {
    s_dirPin = directionPin;
    pinMode(s_dirPin, OUTPUT);
    receiveMode();
    if (s_serial) {
        s_serial->begin(baud);
    }
}

bool poll(MasterPacket &packet) {
    if (!s_serial) {
        return false;
    }

    while (s_serial->available()) {
        int value = s_serial->read();
        if (value < 0) {
            break;
        }

        uint8_t byte = static_cast<uint8_t>(value);

        for (uint8_t i = 0; i < kFrameSize - 1; i++) {
            s_rxBuffer[i] = s_rxBuffer[i + 1];
        }
        s_rxBuffer[kFrameSize - 1] = byte;

        if (s_rxBuffer[0] == kFrameHeader && s_rxBuffer[kFrameSize - 1] == kFrameFooter) {
            uint8_t index = 0;
            index++;
            packet.biasVoltage = get16(&s_rxBuffer[index]); index += 2;
            packet.inputPower = get16(&s_rxBuffer[index]); index += 2;
            packet.outputPower = get16(&s_rxBuffer[index]); index += 2;
            packet.temperature = get16(&s_rxBuffer[index]); index += 2;
            packet.calibrationDone = s_rxBuffer[index++];
            index++;
            packet.rsv[0] = get16(&s_rxBuffer[index]); index += 2;
            packet.rsv[1] = get16(&s_rxBuffer[index]); index += 2;
            packet.rsv[2] = get16(&s_rxBuffer[index]); index += 2;
            packet.rsv[3] = get16(&s_rxBuffer[index]); index += 2;
            return true;
        }
    }

    return false;
}

void send(const SlavePacket &packet) {
    if (!s_serial) {
        return;
    }

    transmitMode();

    uint8_t payload[kFrameSize];
    uint8_t index = 0;

    payload[index++] = kFrameHeader;

    put16(&payload[index], packet.ditherAmp); index += 2;
    put16(&payload[index], packet.ditherFreq); index += 2;
    put16(&payload[index], packet.biasPoint); index += 2;
    put16(&payload[index], packet.ditherOn); index += 2;
    payload[index++] = packet.calibrationRequest;

    for (uint8_t i = 0; i < 4; ++i) {
        put16(&payload[index], packet.rst[i]); index += 2;
    }

    payload[index++] = 0x00;
    payload[index++] = kFrameFooter;

    s_serial->write(payload, kFrameSize);

    uint32_t transmissionTime = (static_cast<uint32_t>(kFrameSize) * 10UL * 1000000UL) / 9600UL;
    delayMicroseconds(transmissionTime + 500);

    receiveMode();

    for (uint8_t n = 0; n < 64 && s_serial->available(); ++n) {
        s_serial->read();
    }
}

}

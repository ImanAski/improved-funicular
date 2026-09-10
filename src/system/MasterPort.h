//
// Created by Iman on 9/10/2026.
//

#ifndef MASTERPORT_H
#define MASTERPORT_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "../../lib/RSProtocol/protocol.h"

namespace MasterPort {

    using SlavePacket = RSProtocol::SlavePacket;

    struct MasterPacket {
        uint16_t biasVoltage;
        uint16_t inputPower;
        uint16_t outputPower;
        uint16_t temperature;
        uint8_t calibrationDone;
        uint16_t rsv[4];
    };

    void init(uint8_t txPin, uint8_t rxPin, uint32_t baud);
    void begin(uint8_t directionPin, uint32_t baud);
    bool poll(MasterPacket &packet);
    void send(const SlavePacket &packet);
}

#endif //MASTERPORT_H

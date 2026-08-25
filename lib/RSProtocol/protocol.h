// //
// // Created by Iman on 8/21/2026.
// //
//
// #ifndef PROTOCOL_H
// #define PROTOCOL_H
//
// #include <Arduino.h>
//
// namespace RSPort {
//     class RS485;
// }
//
// struct MasterPacket {
//     uint8_t header;
//     uint16_t biasVoltage;
//     uint16_t inputPower;
//     uint16_t outputPower;
//     uint16_t temperature;
//     uint8_t calibrationDone;
//     uint16_t rsv[4];
//     uint8_t footer;
// };
//
// struct SlavePacket {
//     uint8_t header;
//     uint16_t ditherAmp;
//     uint16_t ditherFreq;
//     uint16_t biasPoint;
//     uint16_t ditherOn;
//     uint8_t calibrationRequest;
//     uint16_t rst[4];
//     uint8_t footer;
// };
//
//
// class Protocol {
// public:
//
//     static constexpr uint8_t SYNC = 0xAA;
//
//     static constexpr uint8_t TYPE_MASTER = 0x01;
//     static constexpr uint8_t TYPE_SLAVE  = 0x02;
//
//     static constexpr uint8_t MASTER_SIZE = 20;
//     static constexpr uint8_t SLAVE_SIZE  = 20;
//
//     Protocol(RSPort::RS485 &rs485);
//
//     void send(const SlavePacket &packet);
//
//     bool poll(MasterPacket &packet);
//
//     void update();
//
// private:
//
//     RSPort::RS485 &rs485_;
//
//     uint8_t rxBuffer_[64];
//     uint8_t rxIndex_ = 0;
//
//     uint8_t expectedLength_ = 0;
//
//     enum State {
//         WAIT_SYNC,
//         READ_TYPE,
//         READ_LENGTH,
//         READ_PAYLOAD,
//         READ_CRC_LOW,
//         READ_CRC_HIGH
//     };
//
//     State state_ = WAIT_SYNC;
//
//     uint8_t packetType_ = 0;
//     uint16_t receivedCrc_ = 0;
//
//     bool processByte(uint8_t byte);
//
//     bool parseMaster(
//         const uint8_t *data,
//         uint8_t length,
//         MasterPacket &packet
//     );
//
//     uint16_t crc16(
//         const uint8_t *data,
//         size_t length
//     );
//
//     void resetParser();
//
//     static void put16(
//         uint8_t *buffer,
//         uint16_t value
//     );
//
//     static uint16_t get16(
//         const uint8_t *buffer
//     );
// };
//
// #endif

// #include "Protocol.h"
//
// namespace RSPort {
//     class RS485;
// }
//
// Protocol::Protocol(RSPort::RS485 &rs485)
//     : rs485_(rs485)
// {
// }
//
//
// // ============================================================
// // 16-bit serialization
// // ============================================================
//
// void Protocol::put16(
//     uint8_t *buffer,
//     uint16_t value
// )
// {
//     // Little endian
//     buffer[0] = value & 0xFF;
//     buffer[1] = (value >> 8) & 0xFF;
// }
//
//
// uint16_t Protocol::get16(
//     const uint8_t *buffer
// )
// {
//     return
//         static_cast<uint16_t>(buffer[0]) |
//         (static_cast<uint16_t>(buffer[1]) << 8);
// }
//
//
// // ============================================================
// // CRC16
// // ============================================================
//
// uint16_t Protocol::crc16(
//     const uint8_t *data,
//     size_t length
// )
// {
//     uint16_t crc = 0xFFFF;
//
//     for (size_t i = 0; i < length; ++i) {
//
//         crc ^= data[i];
//
//         for (uint8_t j = 0; j < 8; ++j) {
//
//             if (crc & 1) {
//                 crc =
//                     (crc >> 1) ^
//                     0xA001;
//             }
//             else {
//                 crc >>= 1;
//             }
//         }
//     }
//
//     return crc;
// }
//
//
// // ============================================================
// // Send SlavePacket
// // ============================================================
//
// void Protocol::send(
//     const SlavePacket &packet
// )
// {
//     /*
//      * Wire representation:
//      *
//      * SYNC
//      * TYPE
//      * LENGTH
//      * PAYLOAD
//      * CRC16
//      *
//      * SlavePacket payload:
//      *
//      * header              1
//      * ditherAmp           2
//      * ditherFreq          2
//      * biasPoint           2
//      * ditherOn            2
//      * calibrationRequest  1
//      * rst[4]              8
//      * footer              1
//      *
//      * Total = 19 bytes
//      */
//
//     constexpr uint8_t PAYLOAD_SIZE = 19;
//
//     uint8_t frame[
//         3 + PAYLOAD_SIZE + 2
//     ];
//
//     uint8_t index = 0;
//
//     // --------------------------------------------------------
//     // Header
//     // --------------------------------------------------------
//
//     frame[index++] = SYNC;
//     frame[index++] = TYPE_SLAVE;
//     frame[index++] = PAYLOAD_SIZE;
//
//
//     // --------------------------------------------------------
//     // Payload
//     // --------------------------------------------------------
//
//     frame[index++] = packet.header;
//
//     put16(&frame[index], packet.ditherAmp);
//     index += 2;
//
//     put16(&frame[index], packet.ditherFreq);
//     index += 2;
//
//     put16(&frame[index], packet.biasPoint);
//     index += 2;
//
//     put16(&frame[index], packet.ditherOn);
//     index += 2;
//
//     frame[index++] = packet.calibrationRequest;
//
//     for (uint8_t i = 0; i < 4; ++i) {
//
//         put16(
//             &frame[index],
//             packet.rst[i]
//         );
//
//         index += 2;
//     }
//
//     frame[index++] = packet.footer;
//
//
//     // --------------------------------------------------------
//     // CRC
//     // --------------------------------------------------------
//
//     /*
//      * CRC covers:
//      *
//      * TYPE
//      * LENGTH
//      * PAYLOAD
//      *
//      * It does not cover SYNC.
//      */
//
//     const uint16_t crc =
//         crc16(
//             &frame[1],
//             2 + PAYLOAD_SIZE
//         );
//
//     put16(
//         &frame[index],
//         crc
//     );
//
//     index += 2;
//
//
//     // --------------------------------------------------------
//     // Send
//     // --------------------------------------------------------
//
//     rs485_.send(
//         frame,
//         index
//     );
// }
//
//
// // ============================================================
// // Update parser
// // ============================================================
//
// void Protocol::update()
// {
//     while (rs485_.available()) {
//
//         const int value = rs485_.read();
//
//         if (value < 0) {
//             return;
//         }
//
//         processByte(
//             static_cast<uint8_t>(value)
//         );
//     }
// }
//
//
// // ============================================================
// // Parser
// // ============================================================
//
// bool Protocol::processByte(
//     uint8_t byte
// )
// {
//     switch (state_) {
//
//         case WAIT_SYNC:
//
//             if (byte == SYNC) {
//                 state_ = READ_TYPE;
//             }
//
//             break;
//
//
//         case READ_TYPE:
//
//             packetType_ = byte;
//
//             state_ = READ_LENGTH;
//
//             break;
//
//
//         case READ_LENGTH:
//
//             expectedLength_ = byte;
//
//             if (
//                 expectedLength_ == 0 ||
//                 expectedLength_ > sizeof(rxBuffer_)
//             ) {
//                 resetParser();
//                 break;
//             }
//
//             rxIndex_ = 0;
//             state_ = READ_PAYLOAD;
//
//             break;
//
//
//         case READ_PAYLOAD:
//
//             rxBuffer_[rxIndex_++] = byte;
//
//             if (rxIndex_ >= expectedLength_) {
//                 state_ = READ_CRC_LOW;
//             }
//
//             break;
//
//
//         case READ_CRC_LOW:
//
//             receivedCrc_ = byte;
//
//             state_ = READ_CRC_HIGH;
//
//             break;
//
//
//         case READ_CRC_HIGH:
//
//             receivedCrc_ |=
//                 static_cast<uint16_t>(byte) << 8;
//
//             {
//                 uint8_t crcBuffer[66];
//
//                 crcBuffer[0] = packetType_;
//                 crcBuffer[1] = expectedLength_;
//
//                 for (uint8_t i = 0; i < expectedLength_; ++i) {
//                     crcBuffer[2 + i] = rxBuffer_[i];
//                 }
//
//                 const uint16_t calculated =
//                     crc16(
//                         crcBuffer,
//                         2 + expectedLength_
//                     );
//
//                 if (calculated == receivedCrc_) {
//                     // Valid packet.
//                     resetParser();
//                     return true;
//                 }
//             }
//
//             resetParser();
//
//             break;
//     }
//
//     return false;
// }
//
//
// // ============================================================
// // Parse MasterPacket
// // ============================================================
//
// bool Protocol::parseMaster(
//     const uint8_t *data,
//     uint8_t length,
//     MasterPacket &packet
// )
// {
//     constexpr uint8_t EXPECTED_SIZE = 19;
//
//     if (length != EXPECTED_SIZE) {
//         return false;
//     }
//
//     uint8_t index = 0;
//
//     packet.header = data[index++];
//
//     packet.biasVoltage =
//         get16(&data[index]);
//     index += 2;
//
//     packet.inputPower =
//         get16(&data[index]);
//     index += 2;
//
//     packet.outputPower =
//         get16(&data[index]);
//     index += 2;
//
//     packet.temperature =
//         get16(&data[index]);
//     index += 2;
//
//     packet.calibrationDone =
//         data[index++];
//
//     for (uint8_t i = 0; i < 4; ++i) {
//
//         packet.rsv[i] =
//             get16(&data[index]);
//
//         index += 2;
//     }
//
//     packet.footer = data[index++];
//
//     return true;
// }
//
//
// // ============================================================
// // Poll MasterPacket
// // ============================================================
//
// bool Protocol::poll(
//     MasterPacket &packet
// )
// {
//     while (rs485_.available()) {
//
//         const int value = rs485_.read();
//
//         if (value < 0) {
//             return false;
//         }
//
//         const uint8_t byte =
//             static_cast<uint8_t>(value);
//
//         if (!processByte(byte)) {
//             continue;
//         }
//
//         /*
//          * processByte() returned true after CRC validation.
//          *
//          * Now check what type of packet we received.
//          */
//
//         if (packetType_ != TYPE_MASTER) {
//             resetParser();
//             continue;
//         }
//
//         return parseMaster(
//             rxBuffer_,
//             expectedLength_,
//             packet
//         );
//     }
//
//     return false;
// }
//
//
// // ============================================================
// // Parser reset
// // ============================================================
//
// void Protocol::resetParser()
// {
//     state_ = WAIT_SYNC;
//
//     rxIndex_ = 0;
//
//     expectedLength_ = 0;
//
//     receivedCrc_ = 0;
//
//     packetType_ = 0;
// }

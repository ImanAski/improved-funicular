// //
// // Created by Iman on 8/21/2026.
// //
//
// #ifndef PROTOCOL_H
// #define PROTOCOL_H
// #include <cstdint>
//
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
//     Protocol(HardwareSerial &serial, uint8_t directionPin);
//
//     void begin(uint32_t baudrate);
//
//     void send(const SlavePacket &packet);
//
//     void poll(MasterPacket &packet);
//
//     void update();
//
//     bool available() const;
//
//     static bool parseByte(uint8_t byte);
//
// private:
//     HardwareSerial &serial_;
//     uint8_t direction_pin_;
//
//     void setTransmit(bool enable);
// };
//
// #endif //PROTOCOL_H

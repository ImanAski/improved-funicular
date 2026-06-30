//
// Created by Iman on 6/30/2026.
//

#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <Arduino.h>

struct Packet {
    uint8_t type;
    uint8_t len;
    uint8_t payload[32];
};

class Protocol {
public:
    static void sendPacket(const uint8_t *data, uint8_t payloadLen);
    static bool parseByte(uint8_t byte, Packet &out);
    static void parse_packet(struct Packet *packet, uint8_t payload_len);
};


#endif //PROTOCOL_H

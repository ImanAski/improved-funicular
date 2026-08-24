//
// Created by Iman on 6/30/2026.
//

#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <Arduino.h>

enum class PacketType : uint8_t {
    UserAction = 0x03,
    ThemeChange = 0x07
};

enum class PageId : uint8_t {
    Main = 0x01,
    Menu = 0x02,
    Help = 0x03,
};

enum class UserAction : uint8_t {
    ValueChanged = 0x01,
};

enum class ControlId : uint8_t {
    BiasVoltage = 0x00,
    AmpMaxOut = 0x01,
    VoaOutPower = 0x02,
    BiasToggle = 0x03,
    VoaToggle = 0x04,
    LaserToggle = 0x05,
};

struct Packet {
    uint8_t type;
    uint8_t len;
    uint8_t payload[32];
};

struct ControlInfo {
    PageId pageId;
    ControlId controlId;
};

constexpr ControlInfo BiasToggleInfo {
    PageId::Main,
    ControlId::BiasToggle
};

constexpr ControlInfo LaserToggleInfo {
    PageId::Main,
    ControlId::LaserToggle
};

constexpr ControlInfo VoaToggleInfo {
    PageId::Main,
    ControlId::VoaToggle
};

struct ControlMapEntry {
    ControlInfo info;
};

constexpr ControlInfo kControlMap[] = {
     { PageId::Main, ControlId::BiasToggle },
    { PageId::Main, ControlId::LaserToggle },
    { PageId::Main, ControlId::VoaToggle },

    // { "main:biasVoltage", { PageId::Main, ControlId::BiasVoltage } },
    // { "main:ampMaxOut",   { PageId::Main, ControlId::AmpMaxOut } },
    // { "main:voaOutPower", { PageId::Main, ControlId::VoaOutPower } },
};



class Protocol {
public:
    static void send(uint8_t type, const uint8_t *payload, uint8_t len);
    static void sendPacket(const uint8_t *data, uint8_t payloadLen);
    static bool parseByte(uint8_t byte, Packet &out);
    static void parse_packet(struct Packet *packet, uint8_t payload_len);
};


#endif //PROTOCOL_H

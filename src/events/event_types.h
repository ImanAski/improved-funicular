//
// Created by Iman on 6/30/2026.
//

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

enum class EventType {
    ProtocolError,
};

struct Event {
    EventType type;
    uint32_t ts_ms;
    union {
        struct {
            uint8_t tabId;
        } TabChange;
        struct {
            uint8_t code;
        } protocol_error;
    } data;
};

#endif //EVENT_TYPES_H

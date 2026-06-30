//
// Created by Iman on 6/30/2026.
//
#include "serial_input.h"
#include <Arduino.h>
#include "Config.h"
#include "event_types.h"
#include "../../lib/protocol/protocol.h"

namespace {
    uint32_t s_last_error = 0;
    uint32_t s_last_error_ms = 0;
    constexpr uint32_t kProtocolErrorRateLimitMs = 200U;

    void push_protocol_error(uint8_t code) {
        const uint32_t now = millis();
        if ((now - s_last_error_ms) < kProtocolErrorRateLimitMs) {
            return;
        }


        s_last_error_ms = now;
        Event e{};
        // e.type = EventType::
        // e.ts_ms = EventType::
        // e.data.protocol_error.code = EventType::
    }

    void process_stream(Stream &stream, const char *source) {
        while (stream.available()) {
            const uint8_t c = static_cast<uint8_t>(stream.read());

            Packet packet{};
            if (Protocol::parseByte(c, packet)) {
                Event e{};

                switch (packet.type) {
                    case 0x01:
                    case 0x02:
                    case 0x03:
                    case 0x04:
                    case 0x05:
                    case 0x06:
                    default:
                        break;
                }
            }
        }

    }

    void serial_input_init() {
        s_last_error = 0;
        Serial1.begin(UART_BAUDRATE);
    }

    void serial_input_poll() {
        process_stream(Serial1, "UART");
        process_stream(Serial, "USB");
    }
}


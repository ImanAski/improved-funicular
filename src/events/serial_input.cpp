//
// Created by Iman on 6/30/2026.
//
#include "serial_input.h"
#include <Arduino.h>
#include "Config.h"
#include "event_types.h"
#include "../../lib/protocol/protocol.h"
#include "../controller/ParameterController.h"
#include "../system/RSPort.h"

static SoftwareSerial s_rs485Serial(RS48_TX, RS48_RX);
static RSPort::RS485 s_rs485(s_rs485Serial, RS48_IO);

namespace {
    class RS485Stream : public Stream {
    public:
        explicit RS485Stream(RSPort::RS485 &rs) : rs_(rs) {}

        size_t write(uint8_t b) override {
            rs_.send(&b, 1);
            return 1;
        }

        size_t write(const uint8_t *buffer, size_t size) override {
            rs_.send(buffer, size);
            return size;
        }

        int available() override { return rs_.available(); }
        int read() override { return rs_.read(); }
        int peek() override { return -1; }
        void flush() override {}
    private:
        RSPort::RS485 &rs_;
    };
}

static RS485Stream s_rs485Stream(s_rs485);
RSProtocol::Protocol rsProtocol(s_rs485Stream);

namespace SerialInput {
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

    void debug_print_hex_byte(uint8_t v) {
#if PROTOCOL_DEBUG
        const char *hex = "0123456789ABCDEF";
        Serial.print(hex[(v >> 4) & 0x0F]);
        Serial.print(hex[v & 0x0F]);
#else
  (void)v;
#endif
    }

    void debug_print_packet(const char *source, const Packet &packet) {
#if PROTOCOL_DEBUG
        Serial.print("[PKT ");
        Serial.print(source);
        Serial.print("] type=0x");
        debug_print_hex_byte(packet.type);
        Serial.print(" len=");
        Serial.print(packet.len);
        Serial.print(" payload=");
        for (uint8_t i = 0; i < packet.len; ++i) {
            debug_print_hex_byte(packet.payload[i]);
            if (i + 1 < packet.len) {
                Serial.print(' ');
            }
        }
        Serial.println();
#else
        (void)source;
        (void)packet;
#endif
    }

    void process_stream(Stream &stream, const char *source) {
        while (stream.available()) {
            const uint8_t c = static_cast<uint8_t>(stream.read());

            Packet packet{};
            if (Protocol::parseByte(c, packet)) {
                debug_print_packet(source, packet);
                // Serial.printf("Packet type 0x%02X (%u)\n", packet.type, packet.type);
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
        Serial.begin(115200);
        s_rs485.begin(RS485_BAUDRATE);
    }

    void serial_input_poll() {
        RSProtocol::MasterPacket packet{};
        while (rsProtocol.poll(packet)) {
            ParameterController::onMasterPacket(packet);
            ParameterController::sendState();
        }

        process_stream(Serial, "USB");
    }
}


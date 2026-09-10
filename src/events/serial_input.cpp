//
// Created by Iman on 6/30/2026.
//
#include "serial_input.h"
#include <Arduino.h>
#include "Config.h"
#include "event_types.h"
#include "../../lib/protocol/protocol.h"
#include "../system/MasterPort.h"

namespace SerialInput {

    static void process_stream(Stream &stream, const char *source) {
        while (stream.available()) {
            stream.read();
        }
    }

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

    void debug_print_packet(const char *source, const MasterPort::MasterPacket &packet) {
#if PROTOCOL_DEBUG
        Serial.print("[PKT ");
        Serial.print(source);
        Serial.print("] bV=");
        Serial.print(packet.biasVoltage);
        Serial.print(" iP=");
        Serial.print(packet.inputPower);
        Serial.print(" oP=");
        Serial.print(packet.outputPower);
        Serial.print(" T=");
        Serial.print(packet.temperature);
        Serial.println();
#else
        (void)source;
        (void)packet;
#endif
    }

    void serial_input_init() {
        s_last_error = 0;
        Serial.begin(115200);
        MasterPort::init(RS48_TX, RS48_RX, RS485_BAUDRATE);
    }

    void serial_input_poll() {
        MasterPort::MasterPacket packet{};
        while (MasterPort::poll(packet)) {
            debug_print_packet("MASTER", packet);
        }

        process_stream(Serial, "USB");
    }

}

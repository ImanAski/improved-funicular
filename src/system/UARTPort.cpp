//
// Created by Iman on 7/6/2026.
//

#include "UARTPort.h"

#include <cstdint>

#include "protocol.h"
#include "events/serial_input.h"

namespace UARTPort {
    void init() {
        SerialInput::serial_input_init();
    }

    void poll() {
        SerialInput::serial_input_poll();
    }

    void send(uint8_t type, const uint8_t *payload, uint8_t len) {
        Protocol::send(type, payload, len);
    }



}

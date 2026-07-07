//
// Created by Iman on 7/7/2026.
//

#include "ToggleController.h"

#include <array>

#include "protocol.h"

std::array<uint8_t, 4> makeToggle(const ControlInfo c, uint8_t value) {
    return {
        static_cast<uint8_t>(c.pageId),
        static_cast<uint8_t>(c.controlId),
        static_cast<uint8_t>(UserAction::ValueChanged),
        value
    };
}

void ToggleController::toggle(const ControlInfo c, uint8_t value) {
    auto payload = makeToggle(c, value);

    UARTPort::send(
        static_cast<uint8_t>(PacketType::UserAction),
        payload.data(),
        payload.size()
    );
}

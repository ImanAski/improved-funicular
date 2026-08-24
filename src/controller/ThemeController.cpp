//
// Created by Iman on 7/8/2026.
//

#include "ThemeController.h"

#include <array>
#include <cstdint>

#include "protocol.h"
#include "system/UARTPort.h"

std::array<uint8_t, 0> makeThemePacket() {
    return {};
}

void ThemeController::toggleDarkTheme() {
    auto payload = makeThemePacket();


    UARTPort::send(
        static_cast<uint8_t>(PacketType::ThemeChange),
        payload.data(),
        payload.size()
    );
}

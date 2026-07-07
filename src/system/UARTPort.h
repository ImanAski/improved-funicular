//
// Created by Iman on 7/6/2026.
//

#ifndef UARTPORT_H
#define UARTPORT_H
#include <cstdint>

namespace UARTPort {
    void init();
    void poll();
    void send(uint8_t type, const uint8_t *payload, uint8_t len);
}

#endif //UARTPORT_H

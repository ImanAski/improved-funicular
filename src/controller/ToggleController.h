//
// Created by Iman on 7/7/2026.
//

#ifndef TOGGLECONTROLLER_H
#define TOGGLECONTROLLER_H
#include <cstdint>

#include "protocol.h"
#include "system/UARTPort.h"

class ToggleController {
public:
    static void toggle(const ControlInfo c, uint8_t value);
};
#endif //TOGGLECONTROLLER_H

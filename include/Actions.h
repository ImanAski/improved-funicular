//
// Created by Iman on 7/8/2026.
//

#ifndef ACTIONS_H
#define ACTIONS_H
#include <cstdint>

enum class EDIT_VALUE_ACTION : uint8_t {
    Increment = 0x00,
    Decrement = 0x01,
};

enum class BUTTON_ID : uint8_t {
    MenuBtnCalibrate = 0x00,
};

#endif //ACTIONS_H

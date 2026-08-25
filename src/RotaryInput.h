//
// Created by Iman on 7/4/2026.
//

#ifndef ROTARYINPUT_H
#define ROTARYINPUT_H
#include <cstdint>

#include "OneButton.h"
#include "RotaryEncoder.h"

class RotaryInput {
public:
    RotaryInput(uint8_t pinA, uint8_t pinB, uint8_t buttonPin);

    void begin();
    void update();

    bool isPressed();
    int getDiff();

    void attachClick(callbackFunction fn);
    void attachDoubleClick(callbackFunction fn);
    void attachLongPress(callbackFunction fn);
private:
    RotaryEncoder encoder;
    OneButton btn;

    long lastPosition;
    int diff;
    uint8_t _buttonPin;
};

#endif //ROTARYINPUT_H

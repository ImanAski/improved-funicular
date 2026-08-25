//
// Created by Iman on 7/4/2026.
//

#include "RotaryInput.h"


RotaryInput::RotaryInput(uint8_t pinA,
                         uint8_t pinB,
                         uint8_t buttonPin)
    : encoder(pinA, pinB),
      btn(buttonPin, true, true),
      lastPosition(0),
      diff(0),
      _buttonPin(buttonPin)
{ }

void RotaryInput::begin() {
    lastPosition = encoder.getPosition();
}

void RotaryInput::update() {
    encoder.tick();
    btn.tick();

    long pos = encoder.getPosition();

    diff += static_cast<int>(pos - lastPosition);
    lastPosition = pos;
}

bool RotaryInput::isPressed() {
    return digitalRead(_buttonPin) == LOW;
}


int RotaryInput::getDiff() {
    int d = diff;
    diff = 0;
    return d;
}

void RotaryInput::attachClick(callbackFunction fn)
{
    btn.attachClick(fn);
}

void RotaryInput::attachDoubleClick(callbackFunction fn)
{
    btn.attachDoubleClick(fn);
}

void RotaryInput::attachLongPress(callbackFunction fn)
{
    btn.attachLongPressStart(fn);
}

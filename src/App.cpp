//
// Created by Iman on 7/5/2026.
//

#include "App.h"

#include <SerialUSB.h>

#include "system/DisplayPort.h"
#include "system/UARTPort.h"
#include "system/UI.h"

void App::setup() {
    Serial.begin(115200);
    UARTPort::init();
    DisplayPort::init();
    UI::init();
}

void App::loop() {
    DisplayPort::update();
    UARTPort::poll();
}
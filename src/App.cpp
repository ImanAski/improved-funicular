//
// Created by Iman on 7/5/2026.
//

#include "App.h"

#include <SerialUSB.h>

#include "Config.h"
#include "controller/ParameterController.h"
#include "system/DisplayPort.h"
#include "system/MasterPort.h"
#include "system/UARTPort.h"
#include "system/UI.h"

void App::setup() {
    Serial.begin(115200);
    MasterPort::begin(RS48_IO, RS485_BAUDRATE);
    UARTPort::init();
    DisplayPort::init();
    UI::init();
}

void App::loop() {
    DisplayPort::update();
    UARTPort::poll();

    MasterPort::MasterPacket packet{};
    while (MasterPort::poll(packet)) {
        ParameterController::onMasterPacket(packet);
    }

    ParameterController::sendState();
}

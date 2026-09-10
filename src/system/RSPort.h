//
// Created by Iman on 8/21/2026.
//

#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

namespace RSPort {

class RS485 {
public:
    RS485(
        SoftwareSerial &serial,
        uint8_t direction_pin
    );

    void begin(uint32_t baud);

    // RS485
    void send(const uint8_t *data, size_t len);
    void send(uint8_t byte);

    int available();
    int read();

    SoftwareSerial &serial();

    // Command processing
    void processCommands();

    // NeoPixel
    void addPixel(uint8_t pin);
    void setPixel(uint8_t r, uint8_t g, uint8_t b);

    void pixelRed();
    void pixelGreen();
    void pixelBlue();
    void pixelWhite();
    void pixelYellow();
    void pixelPurple();
    void pixelCyan();
    void pixelOff();

private:
    void transmitMode();
    void receiveMode();

    SoftwareSerial &serial_;
    uint8_t direction_pin_;
    uint32_t baud_ = 9600;

    Adafruit_NeoPixel *pixel_ = nullptr;

    String commandBuffer_;
};

}

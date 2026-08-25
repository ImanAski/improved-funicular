#include <Arduino.h>

#include "App.h"
#include "Config.h"
#include "system/RSPort.h"
#include "SoftwareSerial.h"


#define RGB_PIN 16
// Adafruit_NeoPixel pixel(1, RGB_PIN, NEO_GRB + NEO_KHZ800);

uint16_t touchCalData[5] = {53, 1032, 63, 626, 0};

// SoftwareSerial rs(RS48_TX, RS48_RX);
// RSPort::RS485 rs485(rs, RS48_IO);

void setup() {
    // pinMode(RS48_IO, OUTPUT);
    // rs485.addPixel(RGB_PIN);
    // rs485.begin(9600);
    App::setup();
}

void loop() {
    App::loop();
    // rs485.processCommands();
}

//
// Created by Iman on 8/21/2026.
//

#include "RSPort.h"

namespace RSPort {

RS485::RS485(
    SoftwareSerial &serial,
    uint8_t direction_pin
)
    : serial_(serial),
      direction_pin_(direction_pin)
{
}


// ============================================================
// Initialization
// ============================================================

void RS485::begin(uint32_t baud)
{
    baud_ = baud;

    pinMode(direction_pin_, OUTPUT);

    // Start in receive mode.
    receiveMode();

    serial_.begin(baud);

    // Optional pixel.
    if (pixel_ != nullptr) {
        pixelGreen();
    }
}


// ============================================================
// RS485 Direction Control
// ============================================================

void RS485::transmitMode()
{
    // DE = HIGH
    // /RE = HIGH
    digitalWrite(direction_pin_, HIGH);
}


void RS485::receiveMode()
{
    // DE = LOW
    // /RE = LOW
    digitalWrite(direction_pin_, LOW);
}


// ============================================================
// Sending
// ============================================================

void RS485::send(
    const uint8_t *data,
    size_t len
)
{
    if (data == nullptr || len == 0) {
        return;
    }

    transmitMode();

    serial_.write(data, len);

    /*
     * SoftwareSerial::flush() behavior can vary between
     * implementations, so explicitly wait long enough for
     * the bytes to physically leave the UART.
     *
     * This calculation assumes 8N1:
     *
     *   1 start + 8 data + 1 stop = 10 bits/byte
     */
    const uint32_t baud = baud_;

    const uint32_t transmissionTime =
        (static_cast<uint32_t>(len) * 10UL * 1000000UL)
        / baud;

    delayMicroseconds(transmissionTime + 500);

    receiveMode();
}


void RS485::send(uint8_t byte)
{
    send(&byte, 1);
}


// ============================================================
// Raw Serial Access
// ============================================================

int RS485::available()
{
    return serial_.available();
}


int RS485::read()
{
    return serial_.read();
}


SoftwareSerial &RS485::serial()
{
    return serial_;
}


// ============================================================
// Command Processing
// ============================================================

void RS485::processCommands()
{
    while (serial_.available()) {

        const char c =
            static_cast<char>(serial_.read());

        /*
         * Ignore CR.
         *
         * This lets us handle both:
         *
         *   \n
         *   \r\n
         */
        if (c == '\r') {
            continue;
        }

        /*
         * Command finished.
         */
        if (c == '\n') {

            if (commandBuffer_.length() == 0) {
                continue;
            }

            commandBuffer_.trim();
            commandBuffer_.toUpperCase();

            bool valid = true;

            // ------------------------------------------------
            // Commands
            // ------------------------------------------------

            if (commandBuffer_ == "RED") {

                pixelRed();

            }
            else if (commandBuffer_ == "GREEN") {

                pixelGreen();

            }
            else if (commandBuffer_ == "BLUE") {

                pixelBlue();

            }
            else if (commandBuffer_ == "WHITE") {

                pixelWhite();

            }
            else if (commandBuffer_ == "YELLOW") {

                pixelYellow();

            }
            else if (commandBuffer_ == "PURPLE") {

                pixelPurple();

            }
            else if (commandBuffer_ == "CYAN") {

                pixelCyan();

            }
            else if (commandBuffer_ == "OFF") {

                pixelOff();

            }
            else {

                valid = false;

            }


            // ------------------------------------------------
            // Send response
            // ------------------------------------------------

            transmitMode();

            if (valid) {

                serial_.print("OK ");
                serial_.println(commandBuffer_);

            }
            else {

                serial_.print("ERROR UNKNOWN_COMMAND: ");
                serial_.println(commandBuffer_);

            }

            /*
             * Wait for the response to leave the UART.
             *
             * Calculate based on the actual response length.
             */
            const uint32_t responseLength =
                commandBuffer_.length() + 4;

            const uint32_t transmissionTime =
                (responseLength * 10UL * 1000000UL)
                / 9600UL;

            delayMicroseconds(
                transmissionTime + 500
            );

            receiveMode();

            commandBuffer_ = "";

            continue;
        }


        /*
         * Normal character.
         */
        commandBuffer_ += c;

        /*
         * Prevent an accidentally huge command from
         * consuming memory.
         */
        if (commandBuffer_.length() > 64) {
            commandBuffer_ = "";
        }
    }
}


// ============================================================
// NeoPixel
// ============================================================

void RS485::addPixel(uint8_t pin)
{
    if (pixel_ != nullptr) {

        delete pixel_;
        pixel_ = nullptr;
    }

    pixel_ = new Adafruit_NeoPixel(
        1,
        pin,
        NEO_GRB + NEO_KHZ800
    );

    pixel_->begin();

    pixel_->clear();
    pixel_->show();
}


void RS485::setPixel(
    uint8_t r,
    uint8_t g,
    uint8_t b
)
{
    if (pixel_ == nullptr) {
        return;
    }

    pixel_->setPixelColor(
        0,
        pixel_->Color(r, g, b)
    );

    pixel_->show();
}


void RS485::pixelRed()
{
    setPixel(255, 0, 0);
}


void RS485::pixelGreen()
{
    setPixel(0, 255, 0);
}


void RS485::pixelBlue()
{
    setPixel(0, 0, 255);
}


void RS485::pixelWhite()
{
    setPixel(255, 255, 255);
}


void RS485::pixelYellow()
{
    setPixel(255, 255, 0);
}


void RS485::pixelPurple()
{
    setPixel(255, 0, 255);
}


void RS485::pixelCyan()
{
    setPixel(0, 255, 255);
}


void RS485::pixelOff()
{
    setPixel(0, 0, 0);
}

}

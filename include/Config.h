//
// Created by Iman on 6/30/2026.
//

#ifndef CONFIG_H
#define CONFIG_H

#define UART_BAUDRATE 9600
#define UART_TX 0
#define UART_RX 1


#define RS48_TX 0
#define RS48_RX 1
#define RS48_IO 10
#define RS485_BAUDRATE 9600

#define ENC_A_PIN 27
#define ENC_B_PIN 28
#define ENC_BUTTON_PIN 29

#define WIDTH 320
#define HEIGHT 240

// Showing a string
#define BIAS_POINT_DEFAULT 0
#define BIAS_POINT_MIN 0
#define BIAS_POINT_MAX 3
#define BIAS_POINT_STEP 1

#define BIAS_POINT_0 "QUAD+"
#define BIAS_POINT_1 "QUAD-"
#define BIAS_POINT_2 "MAX"
#define BIAS_POINT_3 "NULL"

#define DITHER_AMP_DEFAULT 600
#define DITHER_AMP_MIN 100
#define DITHER_AMP_MAX 2000
#define DITHER_AMP_STEP 100

#define DITHER_FREQ_DEFAULT 1
#define DITHER_FREQ_MIN 1
#define DITHER_FREQ_MAX 5
#define DITHER_FREQ_STEP 1

#define UNIT_BIAS_VOLTAGE " V"
#define UNIT_VPI " V"
#define UNIT_INPUT_POWER " mW"
#define UNIT_OUTPUT_POWER " mW"
#define UNIT_TEMPERATURE " C"
#define UNIT_DITHER_AMP " mV"
#define UNIT_DITHER_FREQ " kHz"
#define UNIT_BIAS_POINT " counts"

#define ENC_A_PIN 27
#define ENC_B_PIN 28
#define ENC_BUTTON_PIN 29

#define WIDTH 320
#define HEIGHT 240

#ifndef PROTOCOL_DEBUG
#define PROTOCOL_DEBUG 1
#endif

#endif //CONFIG_H


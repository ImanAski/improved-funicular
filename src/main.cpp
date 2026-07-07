#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

#include "App.h"
#include "Config.h"
#include "XPT2046_Touchscreen.h"
#include "ui/ui.h"
#include "RotaryInput.h"
#include "ui/screens.h"
#include "ui/actions.h"

// RotaryInput rotary(ENC_B_PIN, ENC_A_PIN, ENC_BUTTON_PIN);

// TFT_eSPI tft = TFT_eSPI();

// static const uint16_t W = 320;
// static const uint16_t H = 240;

uint16_t touchCalData[5] = {53, 1032, 63, 626, 0};

// lv_indev_t *enc_indev = nullptr;

// XPT2046_Touchscreen ts(6);
// #define TIRQ_PIN 22

// enum { SCREENBUFFER_SIZE_PIXELS = W * H / 20 };
//
// static lv_disp_draw_buf_t draw_buf;
// #define BUF_LINES 60
// static lv_color_t buf[W * BUF_LINES];

// void encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
// {
//     int diff = rotary.getDiff();
//     bool pressed = rotary.isPressed();
//
//     Serial.printf("encoder_read: diff=%d pressed=%d\n", diff, pressed);
//
//     data->enc_diff = diff;
//     data->state = pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
// }


// void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
//     uint16_t x = 0, y = 0;
//
//     // Check if touch is detected
//     if (tft.getTouch(&x, &y, 4004)) {
//         // Serial.println("Touch Detected");
//         Serial.printf("%d,%d,%d\n", x, y, tft.getTouchRawZ());
//         // TFT_eSPI getTouch returns calibrated coordinates
//         // But we need to ensure they're within screen bounds
//         data->point.x = x;
//         data->point.y = y;
//         data->state = LV_INDEV_STATE_PR;
//     } else {
//         data->state = LV_INDEV_STATE_REL;
//     }
// }
//
// void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
//     uint32_t w = (area->x2 - area->x1 + 1);
//     uint32_t h = (area->y2 - area->y1 + 1);
//
//     tft.startWrite();
//     tft.setAddrWindow(area->x1, area->y1, w, h);
//     tft.pushColors((uint16_t *) &color_p->full, w * h, true);
//     tft.endWrite();
//
//     lv_disp_flush_ready(disp);
// }

// void action_screen_main_loaded(lv_event_t *e) {
//     lv_indev_set_group(enc_indev, groups.main_grp);
// }
//
// void action_screen_about_loaded(lv_event_t *e) {
//     lv_indev_set_group(enc_indev, groups.help_grp);
// }
//
// void action_screen_menu_loaded(lv_event_t *e) {
//     lv_indev_set_group(enc_indev, groups.menu_grp);
// }


void setup() {
    // Serial.begin(115200);
    // lv_init();
    //
    // tft.begin();
    // tft.setRotation(1);
    // // tft.calibrateTouch(touchCalData, TFT_GREEN, TFT_BLACK, 5);
    // // tft.setTouch(touchCalData);
    // // ts.begin();
    // // ts.setRotation(1);
    //
    // lv_disp_draw_buf_init(&draw_buf, buf, NULL, W * BUF_LINES);
    // rotary.begin();
    // static lv_disp_drv_t disp_drv;
    // lv_disp_drv_init(&disp_drv);
    // /*Change the following line to your display resolution*/
    // disp_drv.hor_res = W;
    // disp_drv.ver_res = H;
    // disp_drv.flush_cb = my_disp_flush;
    // disp_drv.draw_buf = &draw_buf;
    // lv_disp_drv_register(&disp_drv);
    //
    // // static lv_indev_drv_t indev_drv;
    // // lv_indev_drv_init(&indev_drv);
    // // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // // indev_drv.read_cb = my_touchpad_read;
    // // lv_indev_drv_register(&indev_drv);
    //
    // static lv_indev_drv_t i_drv;
    // lv_indev_drv_init(&i_drv);
    // i_drv.type = LV_INDEV_TYPE_ENCODER;
    // i_drv.read_cb = encoder_read;
    // enc_indev = lv_indev_drv_register(&i_drv);
    //
    // ui_create_groups();
    //
    // ui_init();
    App::setup();
}

void loop() {
    // static uint32_t last = millis();
    // uint32_t now = millis();
    //
    // // lv_tick_inc(now - last);
    // // last = now;
    //
    // // lv_timer_handler();
    // // rotary.update();
    // // ui_tick();
    // // delay(5);
    // rotary.update();
    //
    // lv_tick_inc(now - last);
    // last = now;
    // lv_timer_handler();
    //
    // ui_tick();
    App::loop();
}

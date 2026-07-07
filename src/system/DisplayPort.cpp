//
// Created by Iman on 7/5/2026.
//

#include "DisplayPort.h"

#include <lvgl.h>

#include "Config.h"
#include "protocol.h"
#include "RotaryInput.h"
#include "TFT_eSPI.h"
#include "UARTPort.h"
#include "controller/ToggleController.h"
#include "ui/screens.h"
#include "ui/ui.h"

#define BUF_LINES 60

namespace DisplayPort {
    TFT_eSPI tft = TFT_eSPI();

    enum { SCREENBUFFER_SIZE_PIXELS = WIDTH * HEIGHT / 20 };

    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[WIDTH * BUF_LINES];

    RotaryInput rotary(ENC_B_PIN, ENC_A_PIN, ENC_BUTTON_PIN);
    lv_indev_t *enc_indev = nullptr;

    void encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
        int diff = rotary.getDiff();
        bool pressed = rotary.isPressed();

        // Serial.printf("encoder_read: diff=%d pressed=%d\n", diff, pressed);

        data->enc_diff = diff;
        data->state = pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    }

    void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
        uint32_t w = (area->x2 - area->x1 + 1);
        uint32_t h = (area->y2 - area->y1 + 1);

        tft.startWrite();
        tft.setAddrWindow(area->x1, area->y1, w, h);
        tft.pushColors((uint16_t *) &color_p->full, w * h, true);
        tft.endWrite();

        lv_disp_flush_ready(disp);
    }

    extern "C" void action_screen_main_loaded(lv_event_t *e) {
        lv_indev_set_group(enc_indev, groups.main_grp);
        uint8_t payload[] = {

        };
    }

    extern "C" void action_screen_about_loaded(lv_event_t *e) {
        lv_indev_set_group(enc_indev, groups.help_grp);
    }

    extern "C" void action_screen_menu_loaded(lv_event_t *e) {
        lv_indev_set_group(enc_indev, groups.menu_grp);
        uint8_t payload[] = {0x10, 0x33, 0x23, 0x52};
        UARTPort::send(0x01, payload, sizeof(payload));
    }

    extern "C" void action_toggle_changed(lv_event_t *e) {
        auto index = reinterpret_cast<uintptr_t>(lv_event_get_user_data(e));
        const ControlInfo &info = kControlMap[index];

        uint8_t value = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);

        ToggleController::toggle(info, value);
    }

    void init() {
        lv_init();

        tft.begin();
        tft.setRotation(1);

        lv_disp_draw_buf_init(&draw_buf, buf, NULL, WIDTH * BUF_LINES);
        rotary.begin();
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        /*Change the following line to your display resolution*/
        disp_drv.hor_res = WIDTH;
        disp_drv.ver_res = HEIGHT;
        disp_drv.flush_cb = my_disp_flush;
        disp_drv.draw_buf = &draw_buf;
        lv_disp_drv_register(&disp_drv);

        // static lv_indev_drv_t indev_drv;
        // lv_indev_drv_init(&indev_drv);
        // indev_drv.type = LV_INDEV_TYPE_POINTER;
        // indev_drv.read_cb = my_touchpad_read;
        // lv_indev_drv_register(&indev_drv);

        static lv_indev_drv_t i_drv;
        lv_indev_drv_init(&i_drv);
        i_drv.type = LV_INDEV_TYPE_ENCODER;
        i_drv.read_cb = encoder_read;
        enc_indev = lv_indev_drv_register(&i_drv);
    }

    void update() {
        static uint32_t last = millis();
        uint32_t now = millis();

        rotary.update();

        lv_tick_inc(now - last);
        last = now;
        lv_timer_handler();
        ui_tick();
    }
}

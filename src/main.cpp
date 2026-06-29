#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

#include "XPT2046_Touchscreen.h"
#include "ui/ui.h"

#include "examples/widgets/lv_example_widgets.h"

TFT_eSPI tft = TFT_eSPI();

static const uint16_t W = 320;
static const uint16_t H = 240;

uint16_t touchCalData[5] = {300, 3600, 300, 3600, 1};

XPT2046_Touchscreen ts(6);
// #define TIRQ_PIN 22

enum { SCREENBUFFER_SIZE_PIXELS = W * H / 20 };

static lv_disp_draw_buf_t draw_buf;
#define BUF_LINES 40
static lv_color_t buf[W * BUF_LINES];


void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t x = 0, y = 0;

    // Check if touch is detected
    if (tft.getTouch(&x, &y, 4004)) {
        // Serial.println("Touch Detected");
        Serial.printf("%d,%d\n", x, y);
        // TFT_eSPI getTouch returns calibrated coordinates
        // But we need to ensure they're within screen bounds
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
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


void setup() {
    Serial.begin(115200);
    lv_init();

    tft.begin();
    tft.setRotation(1);
    // tft.calibrateTouch(touchCalData, TFT_GREEN, TFT_BLACK, 4);
    // tft.setTouch(touchCalData);
    ts.begin();
    ts.setRotation(1);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, W * BUF_LINES);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = W;
    disp_drv.ver_res = H;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    ui_init();
}

void lv_example_btn_1(void) {
    lv_obj_t *label;

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    // lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    // lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
}

void loop() {
    static uint32_t last = millis();
    uint32_t now = millis();

    lv_tick_inc(now - last);
    last = now;

    lv_timer_handler();
    ui_tick();
    delay(5);
}

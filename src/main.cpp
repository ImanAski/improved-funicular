#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

static const uint16_t W = 240;
static const uint16_t H = 320;

enum { SCREENBUFFER_SIZE_PIXELS = W * H / 20 };

static lv_color_t buf[SCREENBUFFER_SIZE_PIXELS];

void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)px_map, w * h, true);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

/*Set tick routine needed for LVGL internal timings*/
static uint32_t my_tick_get_cb (void) { return millis(); }

void setup()
{
    Serial.begin(115200);

    lv_init();

    Serial.println("BOOT");

    tft.begin();
    tft.setRotation(0);
    // tft.setSwapBytes(true);


    static lv_disp_t *disp;
    disp = lv_display_create(W, H);

    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);

    lv_display_set_buffers(
        disp,
        buf,
        NULL,
        sizeof(buf),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );

    lv_display_set_flush_cb(disp, flush_cb);

    lv_tick_set_cb(my_tick_get_cb);

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "LVGL 9 OK");
    lv_obj_center(label);

    Serial.println("READY");
}

void loop()
{
    lv_timer_handler();
    delay(5);
}

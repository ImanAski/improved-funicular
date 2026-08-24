#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9341 _panel;
    lgfx::Bus_SPI _bus;
    // lgfx::Light_PWM _light;
    // lgfx::Touch_XPT2046 _touch;

public:
    LGFX()
    {
        // SPI Bus
        {
            auto cfg = _bus.config();

            cfg.spi_host = 0;      // RP2040 SPI0
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.freq_read = 16000000;

            // cfg.pin_sclk = 18;
            // cfg.pin_mosi = 19;
            // cfg.pin_miso = 16;
            cfg.pin_dc   = 14;
            cfg.pin_sclk = 7;
            cfg.pin_mosi = 6;
            cfg.pin_miso = 5;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        // LCD
        {
            auto cfg = _panel.config();

            cfg.pin_cs  = 4;
            cfg.pin_rst = 15;

            cfg.panel_width  = SCREEN_WIDTH;
            cfg.panel_height = SCREEN_HEIGHT;

            cfg.memory_width  = SCREEN_WIDTH;
            cfg.memory_height = SCREEN_HEIGHT;

            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;

            // cfg.readable = true;
            cfg.invert = false;
            cfg.rgb_order = false;

            _panel.config(cfg);
        }

        // // Backlight
        // {
        //     auto cfg = _light.config();

        //     cfg.pin_bl = 22;
        //     cfg.invert = false;

        //     _light.config(cfg);
        //     _panel.setLight(&_light);
        // }

        // Touch
        // {
        //     auto cfg = _touch.config();

        //     cfg.x_min = 0;
        //     cfg.x_max = 240;

        //     cfg.y_min = 0;
        //     cfg.y_max = 320;

        //     cfg.spi_host = 0;
        //     cfg.freq = 2500000;

        //     cfg.pin_sclk = 2;
        //     cfg.pin_mosi = 3;
        //     cfg.pin_miso = 4;

        //     cfg.pin_cs = 15;

        //     cfg.bus_shared = true;

        //     // _touch.config(cfg);
        //     // _panel.setTouch(&_touch);
        // }

        setPanel(&_panel);
    }
};
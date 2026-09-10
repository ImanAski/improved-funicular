//
// Created by Iman on 7/5/2026.
//

#ifndef DISPLAYPORT_H
#define DISPLAYPORT_H

#include <lvgl.h>

namespace DisplayPort {
    void init();
    void update();

    lv_indev_t *encoder_indev();
};

#endif //DISPLAYPORT_H

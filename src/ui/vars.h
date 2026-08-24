#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_VERSION = 0,
    FLOW_GLOBAL_VARIABLE_UI_THEME = 1,
    FLOW_GLOBAL_VARIABLE_BIAS_VOLTAGE = 2,
    FLOW_GLOBAL_VARIABLE_SELECTED_ITEM_FOR_SETTING = 3,
    FLOW_GLOBAL_VARIABLE_VPI = 4,
    FLOW_GLOBAL_VARIABLE_IN_PWR = 5,
    FLOW_GLOBAL_VARIABLE_OUT_PWR = 6,
    FLOW_GLOBAL_VARIABLE_DITHER_AMP = 7,
    FLOW_GLOBAL_VARIABLE_DITHER_FREQ = 8
};

// Native global variables

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/
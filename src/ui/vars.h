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
    FLOW_GLOBAL_VARIABLE_AMP_MAX_OUT = 3,
    FLOW_GLOBAL_VARIABLE_VOA_OUT_PWR = 4
};

// Native global variables

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/
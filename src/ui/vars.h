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
    FLOW_GLOBAL_VARIABLE_SELECTED_ITEM_FOR_SETTING = 2
};

// Native global variables

extern const char *get_var_bias_voltage();
extern void set_var_bias_voltage(const char *value);
extern const char *get_var_temperature();
extern void set_var_temperature(const char *value);
extern const char *get_var_vpi();
extern void set_var_vpi(const char *value);
extern const char *get_var_input_power();
extern void set_var_input_power(const char *value);
extern const char *get_var_output_poower();
extern void set_var_output_poower(const char *value);
extern const char *get_var_dither_amp();
extern void set_var_dither_amp(const char *value);
extern const char *get_var_dither_freq();
extern void set_var_dither_freq(const char *value);
extern const char *get_var_bias_point();
extern void set_var_bias_point(const char *value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/
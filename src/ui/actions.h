#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_screen_main_loaded(lv_event_t * e);
extern void action_screen_about_loaded(lv_event_t * e);
extern void action_screen_menu_loaded(lv_event_t * e);

enum {
    ACTION_TOGGLE_CHANGED_PROPERTY_INFO,
};
extern void action_toggle_changed(lv_event_t * e);

extern void action_theme_changed(lv_event_t * e);
extern void action_button_clicked(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/
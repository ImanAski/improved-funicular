#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: btn_toggle
lv_style_t *get_style_btn_toggle_MAIN_DEFAULT();
lv_style_t *get_style_btn_toggle_MAIN_CHECKED();
void add_style_btn_toggle(lv_obj_t *obj);
void remove_style_btn_toggle(lv_obj_t *obj);

// Style: screen_bg
lv_style_t *get_style_screen_bg_MAIN_DEFAULT();
void add_style_screen_bg(lv_obj_t *obj);
void remove_style_screen_bg(lv_obj_t *obj);

// Style: main_item
lv_style_t *get_style_main_item_MAIN_DEFAULT();
void add_style_main_item(lv_obj_t *obj);
void remove_style_main_item(lv_obj_t *obj);

// Style: main_item_text
lv_style_t *get_style_main_item_text_MAIN_DEFAULT();
void add_style_main_item_text(lv_obj_t *obj);
void remove_style_main_item_text(lv_obj_t *obj);

// Style: btn_default
lv_style_t *get_style_btn_default_MAIN_DEFAULT();
void add_style_btn_default(lv_obj_t *obj);
void remove_style_btn_default(lv_obj_t *obj);

// Style: text_default
lv_style_t *get_style_text_default_MAIN_DEFAULT();
void add_style_text_default(lv_obj_t *obj);
void remove_style_text_default(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/
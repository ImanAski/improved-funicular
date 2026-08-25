#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_SPLASH = 1,
    SCREEN_ID_MAIN = 2,
    SCREEN_ID_HELP = 3,
    SCREEN_ID_MENU = 4,
    SCREEN_ID_ITEM_SETTINGS = 5,
    _SCREEN_ID_LAST = 5
};

typedef struct _objects_t {
    lv_obj_t *splash;
    lv_obj_t *main;
    lv_obj_t *help;
    lv_obj_t *menu;
    lv_obj_t *item_settings;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *helpers;
    lv_obj_t *menu_btn;
    lv_obj_t *about_btn;
    lv_obj_t *content;
    lv_obj_t *toggles;
    lv_obj_t *dither_toggle;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *back_button;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *bias_calibration_btn;
    lv_obj_t *helpers_1;
    lv_obj_t *menu_back_btn;
    lv_obj_t *obj9;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *bias_calibration_btn_1;
    lv_obj_t *helpers_2;
    lv_obj_t *menu_back_btn_1;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
} objects_t;

extern objects_t objects;

void create_screen_splash();
void tick_screen_splash();

void create_screen_main();
void tick_screen_main();

void create_screen_help();
void tick_screen_help();

void create_screen_menu();
void tick_screen_menu();

void create_screen_item_settings();
void tick_screen_item_settings();

void create_user_widget_bar(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_bar(void *flowState, int startWidgetIndex);

void create_user_widget_toggle_button(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_toggle_button(void *flowState, int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

// Groups

typedef struct _groups_t {
    lv_group_t *main_grp;
    lv_group_t *help_grp;
    lv_group_t *menu_grp;
    lv_group_t *edit_grp;
} groups_t;

extern groups_t groups;

void ui_create_groups();

// Color themes

enum Themes {
    THEME_ID_DEFAULT,
    THEME_ID_DARK_THEME,
};
enum Colors {
    COLOR_ID_BG_MAIN,
    COLOR_ID_BG_BTN_TOGGLE,
    COLOR_ID_BG_BTN_TOGGLE_CHECKED,
    COLOR_ID_BTN_TOGGLE_TEXT,
    COLOR_ID_MAIN_ITEM_BG,
    COLOR_ID_BORDER_BTN_TOGGLE,
    COLOR_ID_MAIN_ITEM_BORDER,
    COLOR_ID_MAIN_ITEM_TEXT_COLOR,
    COLOR_ID_TEXT_DEFAULT,
};
void change_color_theme(uint32_t themeIndex);
extern uint32_t theme_colors[2][9];

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
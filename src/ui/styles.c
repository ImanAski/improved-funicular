#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: btn_toggle
//

void init_style_btn_toggle_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][1]));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_text_opa(style, 255);
    lv_style_set_text_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][3]));
    lv_style_set_radius(style, 0);
    lv_style_set_shadow_width(style, 0);
    lv_style_set_shadow_ofs_x(style, 0);
    lv_style_set_shadow_ofs_y(style, 0);
    lv_style_set_border_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][5]));
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_width(style, 1);
};

lv_style_t *get_style_btn_toggle_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_btn_toggle_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_btn_toggle_MAIN_CHECKED(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][3]));
    lv_style_set_text_opa(style, 255);
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][2]));
};

lv_style_t *get_style_btn_toggle_MAIN_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_btn_toggle_MAIN_CHECKED(style);
    }
    return style;
};

void add_style_btn_toggle(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_btn_toggle_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_btn_toggle_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
};

void remove_style_btn_toggle(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_btn_toggle_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_btn_toggle_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
};

//
// Style: screen_bg
//

void init_style_screen_bg_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][0]));
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_screen_bg_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_screen_bg_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_screen_bg(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_screen_bg_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_screen_bg(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_screen_bg_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: main_item
//

void init_style_main_item_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][4]));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_text_color(style, lv_color_hex(0xf38521));
    lv_style_set_text_opa(style, 255);
    lv_style_set_pad_left(style, 5);
    lv_style_set_pad_right(style, 5);
    lv_style_set_border_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][6]));
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_width(style, 1);
};

lv_style_t *get_style_main_item_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_main_item_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_main_item(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_main_item_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_main_item(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_main_item_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: main_item_text
//

void init_style_main_item_text_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][7]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_main_item_text_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_main_item_text_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_main_item_text(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_main_item_text_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_main_item_text(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_main_item_text_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: btn_default
//

void init_style_btn_default_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_radius(style, 0);
    lv_style_set_shadow_width(style, 0);
    lv_style_set_shadow_ofs_x(style, 0);
    lv_style_set_shadow_ofs_y(style, 0);
};

lv_style_t *get_style_btn_default_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_btn_default_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_btn_default(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_btn_default_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_btn_default(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_btn_default_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: text_default
//

void init_style_text_default_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][8]));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_text_default_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_text_default_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_text_default(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_text_default_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_text_default(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_text_default_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_btn_toggle,
        add_style_screen_bg,
        add_style_main_item,
        add_style_main_item_text,
        add_style_btn_default,
        add_style_text_default,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_btn_toggle,
        remove_style_screen_bg,
        remove_style_main_item,
        remove_style_main_item_text,
        remove_style_btn_default,
        remove_style_text_default,
    };
    remove_style_funcs[styleIndex](obj);
}
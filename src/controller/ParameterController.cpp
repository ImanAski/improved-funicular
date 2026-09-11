//
// Created by Iman on 8/25/2026.
//

#include "ParameterController.h"

#include <lvgl.h>
#include <string.h>

#include "../../include/Config.h"
#include "../events/serial_input.h"
#include "../system/DisplayPort.h"
#include "../ui/screens.h"
#include "../ui/styles.h"
#include "../ui/vars.h"

namespace ParameterController {

    namespace {
        constexpr uint8_t kParamCount = 3;
        constexpr uint8_t kFrameHeader = 0xFC;
        constexpr uint8_t kFrameFooter = 0x23;

        struct ParamCfg {
            lv_obj_t **valueLabel;
            lv_obj_t **node;
            int32_t min;
            int32_t max;
            int32_t step;
            uint8_t digits;
            uint8_t decimals;
        };

        const ParamCfg kParams[kParamCount] = {
            { &objects.obj8, &objects.bias_point_node, BIAS_POINT_MIN, BIAS_POINT_MAX, BIAS_POINT_STEP, 1, 0 },
            { &objects.obj3, &objects.dither_amr_node, DITHER_AMP_MIN, DITHER_AMP_MAX, DITHER_AMP_STEP, 4, 0 },
            { &objects.obj9, &objects.dither_freq_node, DITHER_FREQ_MIN, DITHER_FREQ_MAX, DITHER_FREQ_STEP, 5, 0 },
        };

        static const char *s_bias_point_labels[] = { BIAS_POINT_0, BIAS_POINT_1, BIAS_POINT_2, BIAS_POINT_3 };

        int32_t s_values[kParamCount] = {
            BIAS_POINT_DEFAULT,
            DITHER_AMP_DEFAULT,
            DITHER_FREQ_DEFAULT,
        };
        struct Telemetry {
            int32_t biasVoltage;
            int32_t temperature;
            int32_t vpi;
            int32_t inputPower;
            int32_t outputPower;
        };
        Telemetry s_telemetry = {};
        lv_obj_t *s_spinboxes[kParamCount] = {};
        ParamId s_active = ParamId::BiasPoint;
        bool s_editing = false;
        bool s_uiReady = false;
        uint16_t s_ditherOn = 0;
        bool s_calibrationPending = false;

        void closeEditor() {
            auto index = static_cast<uint8_t>(s_active);
            lv_obj_t *sb = s_spinboxes[index];
            lv_obj_t *label = *kParams[index].valueLabel;

            s_values[index] = lv_spinbox_get_value(sb);

            lv_obj_add_flag(sb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);

            lv_group_set_editing(groups.edit_grp, false);
            lv_group_remove_all_objs(groups.edit_grp);
            lv_indev_set_group(DisplayPort::encoder_indev(), groups.main_grp);

            s_editing = false;
        }

        void editor_event_cb(lv_event_t *e) {
            lv_event_code_t code = lv_event_get_code(e);
            auto index = static_cast<uint8_t>(s_active);

            if (code == LV_EVENT_RELEASED && s_editing) {
                lv_spinbox_set_step(s_spinboxes[index], kParams[index].step);
            }
            else if (code == LV_EVENT_READY && s_editing) {
                closeEditor();
            }
        }

        void node_click_cb(lv_event_t *e) {
            auto id = static_cast<ParamId>(reinterpret_cast<uintptr_t>(lv_event_get_user_data(e)));
            openEditor(id);
        }
    }

    void init() {
        for (uint8_t i = 0; i < kParamCount; i++) {
            const ParamCfg &cfg = kParams[i];
            lv_obj_t *parent = lv_obj_get_parent(*cfg.valueLabel);
            lv_obj_t *sb = lv_spinbox_create(parent);
            lv_spinbox_set_range(sb, cfg.min, cfg.max);
            lv_spinbox_set_digit_format(sb, cfg.digits, cfg.decimals);
            lv_spinbox_set_step(sb, cfg.step);
            lv_spinbox_set_value(sb, s_values[i]);

            lv_obj_set_style_bg_opa(sb, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(sb, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_all(sb, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_width(sb, 70);
            add_style_main_item_text(sb);

            lv_obj_add_flag(sb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_event_cb(sb, editor_event_cb, LV_EVENT_READY, nullptr);

            s_spinboxes[i] = sb;

            lv_obj_clear_flag(*cfg.node, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_clear_flag(*cfg.node, LV_OBJ_FLAG_CLICK_FOCUSABLE);
            lv_obj_add_event_cb(
                *cfg.node,
                node_click_cb,
                LV_EVENT_SHORT_CLICKED,
                reinterpret_cast<void *>(static_cast<uintptr_t>(i))
            );
        }

        s_uiReady = true;
    }

    void attachMainGroup() {
        lv_group_add_obj(groups.main_grp, objects.bias_point_node);
        lv_group_add_obj(groups.main_grp, objects.dither_freq_node);
        lv_group_add_obj(groups.main_grp, objects.dither_amr_node);
    }

    bool uiReady() {
        return s_uiReady;
    }

    void openEditor(ParamId id) {
        if (!s_uiReady || s_editing) {
            return;
        }

        auto index = static_cast<uint8_t>(id);
        lv_obj_t *sb = s_spinboxes[index];
        lv_obj_t *label = *kParams[index].valueLabel;

        s_active = id;

        lv_spinbox_set_value(sb, s_values[index]);
        lv_spinbox_set_step(sb, kParams[index].step);
        lv_obj_clear_flag(sb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);

        lv_group_remove_all_objs(groups.edit_grp);
        lv_group_add_obj(groups.edit_grp, sb);
        lv_indev_set_group(DisplayPort::encoder_indev(), groups.edit_grp);
        lv_group_set_editing(groups.edit_grp, true);

        s_editing = true;
    }

    void sendState() {
        RSProtocol::SlavePacket sp{};

        sp.header = kFrameHeader;
        sp.ditherAmp = static_cast<uint16_t>(s_values[static_cast<uint8_t>(ParamId::DitherAmp)]);
        sp.ditherFreq = static_cast<uint16_t>(s_values[static_cast<uint8_t>(ParamId::DitherFreq)]);
        sp.biasPoint = static_cast<uint16_t>(s_values[static_cast<uint8_t>(ParamId::BiasPoint)]);
        sp.ditherOn = s_ditherOn;
        sp.calibrationRequest = s_calibrationPending ? 1 : 0;
        sp.footer = kFrameFooter;

        rsProtocol.send(sp);
    }

    void onMasterPacket(const RSProtocol::MasterPacket &packet) {
        // Master scales: temperature/100 C, powers/10000 mW, Vpi/100 V,
        // Vbias/100 V - 12. Stored here as milli-units (m-degrees, micro-watts, mV).
        s_telemetry.biasVoltage = static_cast<int32_t>(packet.biasVoltage) * 10 - 12000;
        s_telemetry.temperature = static_cast<int32_t>(packet.temperature) * 10;
        s_telemetry.vpi = static_cast<int32_t>(packet.rsv[0]) * 10;
        s_telemetry.inputPower = static_cast<int32_t>(packet.inputPower) / 10;
        s_telemetry.outputPower = static_cast<int32_t>(packet.outputPower) / 10;

        if (packet.calibrationDone != 0 && s_calibrationPending) {
            s_calibrationPending = false;
            lv_obj_clear_state(objects.bias_calibration_btn, LV_STATE_DISABLED);
        }
    }

    void requestCalibration() {
        if (!s_uiReady || s_calibrationPending) {
            return;
        }

        s_calibrationPending = true;
        lv_obj_add_state(objects.bias_calibration_btn, LV_STATE_DISABLED);
    }

    void setDitherOn(uint16_t on) {
        s_ditherOn = on;
    }

    extern "C" {

    static char s_txt_biasVoltage[16];
    static char s_txt_temperature[16];
    static char s_txt_vpi[16];
    static char s_txt_inputPower[16];
    static char s_txt_outputPower[16];
    static char s_txt_ditherAmp[16];
    static char s_txt_ditherFreq[16];
    static char s_txt_biasPoint[16];

    static const char *formatMilli(char *buf, size_t size, int32_t milli, const char *unit) {
        int32_t whole = milli / 1000;
        int32_t frac = milli % 1000;
        if (frac < 0) {
            frac = -frac;
        }
        snprintf(buf, size, "%ld.%02ld%s", (long)whole, (long)(frac / 10), unit);
        return buf;
    }

    const char *get_var_bias_voltage(void) {
        return formatMilli(s_txt_biasVoltage, sizeof(s_txt_biasVoltage),
                           s_telemetry.biasVoltage, UNIT_BIAS_VOLTAGE);
    }
    void set_var_bias_voltage(const char *value) { (void)value; }

    const char *get_var_temperature(void) {
        return formatMilli(s_txt_temperature, sizeof(s_txt_temperature),
                           s_telemetry.temperature, UNIT_TEMPERATURE);
    }
    void set_var_temperature(const char *value) { (void)value; }

    const char *get_var_vpi(void) {
        return formatMilli(s_txt_vpi, sizeof(s_txt_vpi),
                           s_telemetry.vpi, UNIT_VPI);
    }
    void set_var_vpi(const char *value) { (void)value; }

    const char *get_var_input_power(void) {
        return formatMilli(s_txt_inputPower, sizeof(s_txt_inputPower),
                           s_telemetry.inputPower, UNIT_INPUT_POWER);
    }
    void set_var_input_power(const char *value) { (void)value; }

    const char *get_var_output_poower(void) {
        return formatMilli(s_txt_outputPower, sizeof(s_txt_outputPower),
                           s_telemetry.outputPower, UNIT_OUTPUT_POWER);
    }
    void set_var_output_poower(const char *value) { (void)value; }

    const char *get_var_dither_amp(void) {
        snprintf(s_txt_ditherAmp, sizeof(s_txt_ditherAmp),
                 "%ld%s", (long)s_values[static_cast<uint8_t>(ParamId::DitherAmp)], UNIT_DITHER_AMP);
        return s_txt_ditherAmp;
    }
    void set_var_dither_amp(const char *value) {
        if (!uiReady()) return;
        s_values[static_cast<uint8_t>(ParamId::DitherAmp)] = atoi(value);
    }

    const char *get_var_dither_freq(void) {
        snprintf(s_txt_ditherFreq, sizeof(s_txt_ditherFreq),
                 "%ld%s", (long)s_values[static_cast<uint8_t>(ParamId::DitherFreq)], UNIT_DITHER_FREQ);
        return s_txt_ditherFreq;
    }
    void set_var_dither_freq(const char *value) {
        if (!uiReady()) return;
        s_values[static_cast<uint8_t>(ParamId::DitherFreq)] = atoi(value);
    }

    const char *get_var_bias_point(void) {
        int idx = s_values[static_cast<uint8_t>(ParamId::BiasPoint)];
        switch (idx) {
            case 0: return BIAS_POINT_0;
            case 1: return BIAS_POINT_1;
            case 2: return BIAS_POINT_2;
            case 3: return BIAS_POINT_3;
            default: return BIAS_POINT_0;
        }
    }
    void set_var_bias_point(const char *value) {
        if (!uiReady()) return;
        for (int i = 0; i < 4; i++) {
            if (strcmp(value, s_bias_point_labels[i]) == 0) {
                s_values[static_cast<uint8_t>(ParamId::BiasPoint)] = i;
                return;
            }
        }
    }

    }
}

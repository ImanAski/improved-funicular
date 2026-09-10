//
// Created by Iman on 8/25/2026.
//

#ifndef PARAMETERCONTROLLER_H
#define PARAMETERCONTROLLER_H

#include <cstdint>

#include "../../lib/RSProtocol/protocol.h"

namespace ParameterController {

    enum class ParamId : uint8_t {
        BiasPoint = 0,
        DitherAmp = 1,
        DitherFreq = 2,
    };

    void init();
    void attachMainGroup();
    void openEditor(ParamId id);

    bool uiReady();

    void requestCalibration();

    void onMasterPacket(const RSProtocol::MasterPacket &packet);
    void sendState();
    void setDitherOn(uint16_t on);
}

#endif //PARAMETERCONTROLLER_H

//
// Created by Iman on 6/30/2026.
//

#ifndef SERIAL_INPUT_H
#define SERIAL_INPUT_H

#include "../../lib/RSProtocol/protocol.h"

namespace SerialInput {
    void serial_input_init(void);

    void serial_input_poll(void);
}

extern RSProtocol::Protocol rsProtocol;

#endif //SERIAL_INPUT_H

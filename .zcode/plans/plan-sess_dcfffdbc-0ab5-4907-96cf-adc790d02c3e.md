## Implementation Plan

### Change 1 — Respond to every master message with the full state

Keep the existing SlavePacket format (ditherAmp, ditherFreq, biasPoint, ditherOn, calibrationRequest). The device will send ONLY as a response to a master packet — no other sends.

1. **`src/controller/ParameterController.h`** — add a new public function: `void sendState();`
2. **`src/controller/ParameterController.cpp`** — expose the existing `sendSlavePacket()` (line 62, currently in the anonymous namespace) as the public `sendState()` (packet format unchanged). **Remove the existing internal calls** to `sendSlavePacket()` in `closeEditor()`, `requestCalibration()`, and `setDitherOn()` — per your request, no extra sends for now; the local UI events just update state.
3. **`src/events/serial_input.cpp`** — in `serial_input_poll()` (lines 123-130), call `ParameterController::sendState()` right after `ParameterController::onMasterPacket(packet)`, so every received master packet gets a state response. DE/RE direction control is already handled transparently by `RS485Stream::write → RS485::send`, so no change is needed there.

### Change 2 — Scaling coefficients for master packet values

In `ParameterController::onMasterPacket()` (lines 181-194), store raw values as fixed-point milli-units (exact integer math, no precision loss):

| Field | Formula (raw → stored) | Storage unit | Display |
|---|---|---|---|
| temperature | `raw × 10` | milli-degrees | `xx.xx C` |
| inputPower | `raw / 10` | micro-watts (milli-mW) | `x.xx mW` |
| outputPower | `raw / 10` | micro-watts (milli-mW) | `x.xx mW` |
| vpi (`rsv[0]`) | `raw × 10` | milli-volts | `x.xx V` |
| biasVoltage (Vbias) | `raw × 10 − 12000` | milli-volts | `x.xx V` (range −12..+12) |

Conversions: raw÷100 °C = raw×10 milli-degrees; raw÷10000 mW = raw÷10 µW; raw÷100 V = raw×10 mV; Vbias = raw÷100 − 12 V = raw×10 − 12000 mV.

### Display changes (2 decimal places)

- Add a `formatMilli(buf, size, value, unit)` helper in the `extern "C"` block that formats an `int32_t` milli-unit value with sign, integer part, and **2 decimal places** (only Vbias can be negative).
- The getters `get_var_temperature`, `get_var_vpi`, `get_var_input_power`, `get_var_output_poower`, and `get_var_bias_voltage` switch to this helper.
- **`include/Config.h`**: change `UNIT_BIAS_VOLTAGE` from `" mV"` to `" V"` (since Vbias is now in the −12..+12 V range). All other units unchanged.

### Notes
- The response send busy-waits ~25 ms at 9600 baud (existing behavior in `RSPort::send`); since the master sends first and waits for the response in the same exchange, this causes no issue.
- The slave frame format (19-byte payload + CRC) stays untouched, so the master side needs no changes.
# improved-funicular — RP2040 Modulator Controller

> LVGL 8.4 + EEZ Studio (eez-flow) UI for a Waveshare RP2040-Zero driving a 320×240 ILI9341 via TFT_eSPI, with rotary-encoder navigation and RS-485 slave protocol to a master modulator board.

| Property | Value |
|---|---|
| **MCU / Board** | Raspberry Pi RP2040 — `waveshare_rp2040_zero` |
| **Framework** | Arduino (via PlatformIO) |
| **Display** | ILI9341 320×240 SPI (`TFT_eSPI`, `Adafruit_ILI9341` compat) |
| **Touch** | XPT2046 (wired but encoder is primary input) |
| **UI Engine** | LVGL 8.4 + [EEZ Studio / eez-flow](https://www.envox.eu/eez-studio/) |
| **Input** | EC11 Rotary Encoder + Push-button (`RotaryEncoder` + `OneButton`) |
| **Comm** | RS-485 half-duplex over `SoftwareSerial` + `Adafruit NeoPixel` status LED |
| **Toolchain** | PlatformIO, LVGL, TFT_eSPI |

---

## Table of Contents

1. [Quick Start](#quick-start)
2. [Project Structure](#project-structure)
3. [Folder Purposes](#folder-purposes)
4. [Configuration — `include/Config.h`](#configuration--includeconfigh)
5. [UI Development — The `src/ui/` / eez-flow Pipeline](#ui-development--the-srcui--eez-flow-pipeline)
6. [Firmware Architecture](#firmware-architecture)
7. [Build / Flash / Monitor](#build--flash--monitor)
8. [Semantic Versioning History](#semantic-versioning-history)

---

## Quick Start

```powershell
# 1. Clone
git clone https://github.com/<you>/improved-funicular.git
Set-Location improved-funicular

# 2. Open in VS Code (PlatformIO extension) or:
pio run                  # build
pio run --target upload  # flash to COM18 (see platformio.ini)
pio device monitor --baud 115200
```

> **Upload port** is hard-coded to `COM18` in `platformio.ini:16`. Change it or pass `--upload-port COMx` if your board enumerates differently.  
> **TFT calibration** data lives in `src/main.cpp:touchCalData` — re-run TFT_eSPI touch calibration if you swap displays.

---

## Project Structure

```
improved-funicular/
├── platformio.ini              # PlatformIO env: rp2040 (Arduino), libs, build_flags
├── README.md                   # This file
├── .gitignore                  # Ignores .pio/, compile_commands.json, IDE user files
│
├── include/                    # Global compile-time configuration (headers on -I include)
│   ├── Config.h                # ← SINGLE SOURCE OF TRUTH — pins, ranges, defaults, units
│   ├── Actions.h               # EDIT_VALUE_ACTION / BUTTON_ID enums (app-level)
│   ├── User_Setup.h            # TFT_eSPI hardware wiring override
│   └── lv_conf.h               # LVGL 8.4 feature config (mem, fonts, widgets, drivers)
│
├── src/
│   ├── main.cpp                # Arduino entry: setup()/loop() → App::setup/loop
│   ├── App.h / App.cpp         # Top-level orchestrator (Serial, UARTPort, DisplayPort)
│   ├── RotaryInput.h/.cpp      # Encoder + button abstraction (RotaryEncoder + OneButton)
│   │
│   ├── controller/             # Business logic / state
│   │   ├── ParameterController.h/.cpp  # Dither amp/freq, Bias Point, telemetry, calibration
│   │   └── ThemeController.h/.cpp      # Light ↔ Dark LVGL theme switch
│   │
│   ├── system/                 # Hardware abstraction & bridges
│   │   ├── DisplayPort.h/.cpp  # LVGL init, TFT_eSPI flush, encoder lv_indev, ui_init/tick
│   │   ├── RSPort.h/.cpp       # RS-485 half-duplex wrapper + NeoPixel helpers
│   │   └── UARTPort.h/.cpp     # USB Serial protocol framing
│   │
│   ├── events/
│   │   ├── event_types.h       # EventType / Event union
│   │   └── serial_input.h/.cpp # SoftwareSerial→RSProtocol glue, MasterPacket polling
│   │
│   └── ui/                     # ★ GENERATED — DO NOT HAND-EDIT (see EEZ-Flow section)
│       ├── ui.h / ui.c         # Asset blob (7768 B), native_vars[], actions[], ui_init/tick
│       ├── screens.h/.c        # Screen/widget tree (Splash, Main, Help, Menu), LVGL groups
│       ├── styles.h/.c         # LVGL style definitions (screen_bg, main_item, btn_*)
│       ├── vars.h              # Flow global vars (VERSION, UI_THEME, …) + native var accessors
│       ├── actions.h           # Event callbacks: action_screen_*_loaded, toggle, theme, button
│       ├── images.h/.c         # Image descriptors
│       ├── fonts.h             # Font declarations
│       ├── structs.h           # Container for EEZ types (vars.h wrapper)
│       ├── eez-flow.h/.cpp     # EEZ runtime (alloc, Value, FlowState, ~370 kB engine)
│       ├── ui_image_*.c        # Embedded image assets (CQST logo, laser icons)
│       └── .eez-project-build  # Manifest of files emitted by EEZ Studio on last export
│
├── lib/
│   ├── RSProtocol/             # Active half-duplex protocol (MasterPacket ↔ SlavePacket)
│   │   ├── protocol.h/.cpp     # CRC16, SYNC=0xAA, TYPE_MASTER/SLAVE, poll()/send()
│   │   └── README.md           # Packet structure documentation
│   └── protocol/               # Legacy/unified framing (Packet {type,len,payload})
│       └── protocol.h/.cpp
│
├── .vscode/                    # VS Code settings (shared)
│   └── settings.json / c_cpp_properties.json / extensions.json / launch.json
├── .idea/                      # JetBrains shared project files (workspace.xml ignored)
└── .pio/ / .pio/libdeps/       # PlatformIO build cache (git-ignored)
```

---

## Folder Purposes

| Path | Purpose | Edit? |
|---|---|---|
| **`include/`** | Compile-time headers added via `build_flags = -I include`. Central place for anything that must be visible to every translation unit without relative-path includes. | Yes — `Config.h` is the intended user-tuning file. |
| **`include/Config.h`** | Pins, baudrates, display size, min/max/step/defaults for each controllable parameter, unit strings, debug flags. See dedicated section below. | **Primary knob.** |
| **`include/User_Setup.h`** | Overrides TFT_eSPI defaults via macros (`ILI9341_DRIVER`, `TFT_MISO/MOSI/SCLK/CS/DC/RST`, `SPI_FREQUENCY`). PlatformIO copies it over the library's `User_Setup.h` via `-I include` ordering. | Edit when you rewire SPI. |
| **`include/lv_conf.h`** | Full LVGL configuration (≈1060 lines). Fonts enabled (Montserrat 8/10/12/14/24/30), `LV_MEM_SIZE 128 KB`, `LV_COLOR_DEPTH 16`, ILI9341 driver enabled, demos enabled. Rarely touched. | Only if you add widgets/fonts or hit RAM limits. |
| **`include/Actions.h`** | App-level enums (`EDIT_VALUE_ACTION::Increment/Decrement`, `BUTTON_ID::MenuBtnCalibrate`) decoupled from EEZ-generated `actions.h`. | Add IDs when you add new encoder-editable actions. |
| **`src/`** | Application source. `main.cpp` is intentionally thin — it delegates to `App`. | Yes. |
| **`src/App.*`** | `App::setup()` → `Serial@115200` + `UARTPort::init()` + `DisplayPort::init()`; `App::loop()` → `DisplayPort::update()` + `UARTPort::poll()`. No business logic here. | Add new subsystem inits here. |
| **`src/RotaryInput.*`** | Wraps `RotaryEncoder` (quadrature) + `OneButton` (click/double/long). Exposes `getDiff()` (encoder delta) and `isPressed()`. Fed to LVGL's `LV_INDEV_TYPE_ENCODER` in `DisplayPort`. | Replace if you change encoder library. |
| **`src/controller/`** | State + mutation. Does **not** own LVGL objects directly — it drives `vars.h` native variables and reads/writes `objects.*` handles exported by `screens.h`. | Core of feature work. |
| **`src/controller/ParameterController`** | Manages 3 parameters (Bias Point, Dither Amp, Dither Freq), each with `{min,max,step,digits,decimals}` in a `ParamCfg` table pointing at `objects.obj7/obj9/obj8` labels and `objects.bias_point_node` etc. Creates `lv_spinbox` editors on-the-fly, swaps `main_grp ↔ edit_grp` for encoder editing, formats telemetry (`biasVoltage/temperature/Vpi/inputPower/outputPower`) via `get_var_*()` accessors called by eez-flow, and implements RS-485 `sendState()` / `onMasterPacket()`. | Edit to add/remove parameters. |
| **`src/controller/ThemeController`** | Single call `toggleDarkTheme()` switching between `THEME_ID_DEFAULT` ↔ `THEME_ID_DARK_THEME` (defined in `screens.h:theme_colors[2][9]`). | |
| **`src/system/DisplayPort`** | LVGL lifecycle: `lv_init()`, `tft.begin()+setRotation(3)`, `lv_disp_draw_buf_init` (320×60 ring), `lv_disp_drv_register` with `my_disp_flush → tft.pushColors`, `lv_indev_drv_register` with `encoder_read → RotaryInput::getDiff/isPressed`, `ui_create_groups(); ui_init(); ParameterController::init();` Bridges EEZ actions (`action_screen_*_loaded`, `action_toggle_changed`, `action_button_clicked`) by setting the correct `lv_group` and delegating to controllers. | Touch/encoder/display changes here. |
| **`src/system/RSPort`** | Half-duplex RS-485: holds a `SoftwareSerial &` + `direction_pin`, `transmitMode()/receiveMode()` toggling, `send()/available()/read()`, and a `processCommands()` text-mode parser plus NeoPixel status (`pixelRed/Green/Blue/…`). | Rewire DE/RE pin or swap to HW UART here. |
| **`src/system/UARTPort`** | USB-Serial framing: `init()/poll()/send(type,payload,len)`. | |
| **`src/events/serial_input`** | Glue: instantiates `SoftwareSerial(RS48_TX,RX) + RSPort + RSProtocol::Protocol`, exposes global `RSProtocol::Protocol rsProtocol` for `ParameterController::sendState()`. `serial_input_poll()` → `rsProtocol.poll(MasterPacket)` → `ParameterController::onMasterPacket()` + `sendState()`. Also contains legacy `Protocol::parseByte` hex-dump for USB debug (`PROTOCOL_DEBUG`). | Protocol upgrade touches this + `lib/RSProtocol`. |
| **`src/ui/`** | **Generated UI.** Treat as build artifact. See next section. | **Do not edit by hand.** |
| **`lib/RSProtocol/`** | Active RS-485 wire format: `MasterPacket` (biasVoltage, inputPower, outputPower, temperature, calibrationDone, rsv[4]) ↔ `SlavePacket` (ditherAmp, ditherFreq, biasPoint, ditherOn, calibrationRequest, rst[3]), framing `header 0xFC / footer 0x23`, `CRC16`, `SYNC 0xAA`. Header file is the canonical spec — `README.md` duplicates it. | Edit when wire format changes (keep master firmware in sync). |
| **`lib/protocol/`** | Older generic `Packet {type, len, payload[]}` parser used by `events/serial_input` USB debug path and `UARTPort/ThemeController`. Retained for compatibility. | Deprecated — prefer `RSProtocol`. |
| **`platformio.ini`** | Single env `rp2040`: `platform=raspberrypi`, `board=waveshare_rp2040_zero`, `framework=arduino`, `monitor_speed=115200`, `upload_port=COM18`, `build_flags` for LVGL simple-include and `include/` path, `lib_deps` (TFT_eSPI, Adafruit ILI9341, XPT2046, lvgl@8.4, RotaryEncoder, NeoPixel, OneButton). | Update `upload_port` and `lib_deps` here. |
| **`README.md` / `lib/RSProtocol/README.md`** | Project and protocol documentation. | Keep in sync with wire format. |
| **`.pio/` / `.pio/libdeps/` / `compile_commands.json`** | Build cache, downloaded libs, and clangd DB. Git-ignored via `.gitignore`. | `pio run --target clean` to wipe. |
| **`.idea/` / `.vscode/`** | Shared IDE config. User-local files (`workspace.xml`, `shelf/`, `.zcode/`) are ignored. | Commit only shared settings. |

---

## Configuration — `include/Config.h`

`Config.h` is the **single header you edit to retarget hardware or tune parameters** without touching logic. It is guarded by `#ifndef CONFIG_H` and injected into every TU via `-I include`.

### 1. Communication

```cpp
#define UART_BAUDRATE 9600
#define UART_TX 0
#define UART_RX 1

#define RS48_TX 0          // ⚠ shares pins with UART_TX/RX — intentional: RS-485 *is* the UART
#define RS48_RX 1          //    on the RP2040 Zero this build uses SoftwareSerial on the same pins
#define RS48_IO 10         // DE/RE direction pin (HIGH = TX, LOW = RX)
#define RS485_BAUDRATE 9600
```

> **Note:** `UART_*` and `RS48_*` aliases overlap on pins 0/1. `events/serial_input.cpp` constructs `SoftwareSerial(RS48_TX, RS48_RX)` — the `UART_*` defines are kept for documentation / future HW-UART swap. If you move to `Serial1`/`Serial2`, update both blocks.

### 2. Input Hardware

```cpp
#define ENC_A_PIN 27
#define ENC_B_PIN 28
#define ENC_BUTTON_PIN 29
```

Wired to `RotaryInput(ENC_A, ENC_B, ENC_BUTTON_PIN)` and exposed to LVGL as `LV_INDEV_TYPE_ENCODER`. Swapping pins only requires changing these three lines.

### 3. Display

```cpp
#define WIDTH  320
#define HEIGHT 240
```

Must match `TFT_WIDTH/HEIGHT` in `User_Setup.h` and `lv_disp_drv.hor_res/ver_res` in `DisplayPort.cpp`. Rotation is fixed to `3` (landscape, USB to the right) in `DisplayPort::init()`.

### 4. Tunable Parameters — Ranges & Defaults

Each parameter is a `{DEFAULT, MIN, MAX, STEP}` tuple plus a human-readable label macro. Values are consumed by `controller/ParameterController.cpp`'s `kParams[]` table:

```cpp
// Bias Point (enum-like: QUAD+ / QUAD- / MAX / NULL)
#define BIAS_POINT_DEFAULT 0
#define BIAS_POINT_MIN     0
#define BIAS_POINT_MAX     3
#define BIAS_POINT_STEP    1
#define BIAS_POINT_0 "QUAD+"
#define BIAS_POINT_1 "QUAD-"
#define BIAS_POINT_2 "MAX"
#define BIAS_POINT_3 "NULL"

// Dither Amplitude (mV)
#define DITHER_AMP_DEFAULT 600
#define DITHER_AMP_MIN     100
#define DITHER_AMP_MAX     2000
#define DITHER_AMP_STEP    100

// Dither Frequency (kHz)
#define DITHER_FREQ_DEFAULT 1
#define DITHER_FREQ_MIN     1
#define DITHER_FREQ_MAX     5
#define DITHER_FREQ_STEP    1
```

`ParameterController` creates an `lv_spinbox` per parameter with `set_range(MIN,MAX)`, `set_digit_format(digits,decimals)`, `set_step(STEP)`, and `set_value(DEFAULT)`. To add a new parameter: add its `*_DEFAULT/MIN/MAX/STEP` here, add a row to `kParams[]` pointing at the new `objects.*` label/node, and add a `get_var_*` accessor.

### 5. Unit Strings

```cpp
#define UNIT_BIAS_VOLTAGE " V"
#define UNIT_VPI          " V"
#define UNIT_INPUT_POWER  " mW"
#define UNIT_OUTPUT_POWER " uW"
#define UNIT_TEMPERATURE  " C"
#define UNIT_DITHER_AMP   " mV"
#define UNIT_DITHER_FREQ  " kHz"
#define UNIT_BIAS_POINT   " counts"
```

Appended by `ParameterController::formatMilli()` when producing `get_var_*()` strings for LVGL labels. Change here to re-label the UI without touching flow.

### 6. Debug

```cpp
#ifndef PROTOCOL_DEBUG
#define PROTOCOL_DEBUG 1
#endif
```

`1` → `serial_input.cpp` hex-dumps every parsed `Packet` to `Serial`. Set to `0` (or define `PROTOCOL_DEBUG=0` in `platformio.ini:build_flags`) to silence and save flash/time.

> **Known quirk:** `Config.h` duplicates `ENC_A_PIN/B/C` and `WIDTH/HEIGHT` defines (lines 18-23 and 55-60). The values are identical so the duplicate is harmless but will trigger `-Wmacro-redefined` if warnings are escalated. A cleanup can remove the second block.

### Typical workflow

1. Edit `Config.h` (or `User_Setup.h` for SPI wiring).
2. `pio run` — no other file needs touching for pin/range changes.
3. Flash and verify on serial monitor (`PROTOCOL_DEBUG`).

---

## UI Development — The `src/ui/` / eez-flow Pipeline

`src/ui/` is **not hand-written**. It is the **EEZ Studio export** for the LVGL + eez-flow project. The single source of truth is the `.eez` project opened in EEZ Studio; `src/ui/*.c/.h` are code-generation artifacts.

### Mental model

```
┌─────────────────┐      Build (Export)      ┌──────────────────────────┐
│  EEZ Studio     │  ─────────────────────►  │  src/ui/                  │
│  (visual editor)│                          │  ui.c / screens.c /       │
│  Project: *.eez │                          │  styles.c / vars.h /      │
│  Screens + Flow │                          │  eez-flow.h/.cpp runtime  │
└─────────────────┘                          └────────────┬─────────────┘
                                                          │ ui_init()
                                                          │ ui_tick()
                                                          ▼
                                             ┌──────────────────────────┐
                                             │  src/system/DisplayPort  │
                                             │  src/controller/*        │
                                             │  (hand-written bridge)   │
                                             └──────────────────────────┘
```

### What each generated file is

| File | Role | Hand-edit? |
|---|---|---|
| `ui.h/.c` | **Top-level.** Holds the `assets[7768]` LVGL/flow binary blob, `native_vars[]` binding table, `actions[]` dispatch table, `ui_init()` (`eez_flow_init(...)`) and `ui_tick()` (`eez_flow_tick() + tick_screen(...)`). | Never. |
| `screens.h/.c` | Widget tree for `Splash / Main / Help / Menu` + user widgets `bar`/`toggle_button`. Declares `objects_t objects` (every `lv_obj_t*` by name) and `groups_t groups` (LVGL `lv_group_t*` per screen + `edit_grp`). Also `ScreensEnum`, `Themes`, `Colors`, `theme_colors[2][9]`. Code-gen creates/destroys/layouts all LVGL objects and emits `event_handler_cb_*` stubs that forward to flow or `actions.h` callbacks. | Never — re-export. |
| `styles.h/.c` | LVGL styles: `btn_toggle`, `screen_bg`, `main_item`, `main_item_text`, `btn_default`, `text_default` (with `MAIN_DEFAULT / MAIN_CHECKED / MAIN_FOCUSED` variants). | Never — edit in EEZ Studio Style editor. |
| `vars.h` | **Contract between flow and firmware.** `FlowGlobalVariables` enum (`VERSION`, `UI_THEME`, `SELECTED_ITEM_FOR_SETTING`) + `extern const char *get_var_*() / set_var_*()` for 8 native vars (bias_voltage, temperature, vpi, input_power, output_poower [sic], dither_amp, dither_freq, bias_point). Firmware **must** implement these (see `ParameterController.cpp` `extern "C"` block). | Only to add a var — but prefer adding it in EEZ Studio so the export updates it. |
| `actions.h` | **Contract for events.** `action_screen_main_loaded / about / menu`, `action_toggle_changed`, `action_theme_changed`, `action_button_clicked` (+ `ACTION_TOGGLE_CHANGED_PROPERTY_INFO`). Firmware implements them in `DisplayPort.cpp`. | Same — add actions in Studio, implement in `DisplayPort.cpp`. |
| `images.h/.c` + `ui_image_*.c` | Image descriptors + raw pixel data (CQST logo ~2.6 MB, laser icons ~180 KB each). | Never. |
| `fonts.h` | Font declarations (Montserrat 8/10/12/14/24/30 as per `lv_conf.h`). | Generated. |
| `structs.h` | Thin wrapper including `eez-flow.h` + `vars.h`. | Generated. |
| `eez-flow.h/.cpp` | EEZ runtime (~160 KB header + 370 KB cpp): allocator, `Value` variant, flow engine, LVGL bindings, expression evaluator. Version stamped `April 6, 2026 / commit d9ff0db4`. | Never. |
| `.eez-project-build` | JSON manifest listing every file emitted on last export — used by incremental build check. | Generated. |

### Development workflow

#### Prerequisites

- [EEZ Studio](https://www.envox.eu/eez-studio/) (the project was last edited with a 2026-04-06 eez-framework build; any recent EEZ Studio ≥ 2025 works for LVGL 8.4).
- This repo checked out. The EEZ project file (`.eez`) is expected to sit **next to** `src/ui/` or at the repo root — if absent, create a new EEZ LVGL project and point its **Build → Output directory** to `src/ui`.

#### Everyday loop

1. **Open the project in EEZ Studio**
   - `File → Open Project` → select `*.eez` (or `File → Import` → point at `src/ui/ui.c` to reconstitute).

2. **Edit screens visually**
   - `Screens` panel → `Splash` (boot spinner), `Main` (HOME with 5 telemetry rows + 3 editable parameter nodes + helpers sidebar), `Help` (About), `Menu` (Theme switch + Calibration).
   - Drag LVGL widgets; set `Style` (`screen_bg`, `main_item`, etc.); assign `Group` membership (`main_grp`, `help_grp`, `menu_grp`, `edit_grp`) — groups are what the encoder navigates.

3. **Edit flow (logic)**
   - `Flow` tab → wire events: `LV_EVENT_CLICKED` on `menu_btn` → flow action `→ Show Screen: Menu`; `LV_EVENT_VALUE_CHANGED` on `dither_toggle` → `action_toggle_changed`; `LV_EVENT_SCREEN_LOADED` on `Main` → `action_screen_main_loaded`.
   - **Variables:** `Project → Variables` → Global (`VERSION`, `UI_THEME`) and Native (`bias_voltage`, …). Adding a native var adds a `get_var_*`/`set_var_*` entry in `vars.h`.
   - **Actions:** `Project → Actions` → add e.g. `action_my_new_feature`; EEZ will add it to `actions.h` and `ui.c:actions[]` dispatch table. You then implement it in `src/system/DisplayPort.cpp` (or any TU) as `extern "C" void action_my_new_feature(lv_event_t *e) { … }`.

4. **Themes & Styles**
   - `Project → Themes` (Default / Dark Theme) and `Styles` — colors live in `screens.h:theme_colors[2][9]` (`BG_MAIN`, `BG_BTN_TOGGLE`, `MAIN_ITEM_BG`, …). Changing a color here updates `ui.c:assets[]` and `styles.c`.

5. **Build / Export**
   - `Build → Build` (or `Ctrl+B`). EEZ validates the flow and overwrites `src/ui/*.c/.h`. **Do not edit those files manually** — diff them to confirm the export, then commit.

6. **Wire in firmware**
   - Implement new `get_var_*` in `src/controller/ParameterController.cpp` (follow the `formatMilli()` pattern).
   - Implement new `action_*` in `src/system/DisplayPort.cpp` (follow `action_screen_main_loaded` pattern: set `lv_indev_set_group`, delegate to controller).
   - If the new screen needs encoder focus, add `lv_group_add_obj(groups.<new>_grp, objects.<widget>)` in the new screen's `LV_EVENT_SCREEN_LOAD_START` handler — or let EEZ generate it if you configured groups in Studio.

7. **Build firmware**
   - `pio run` — the generated `src/ui/*.c` are compiled as regular sources. If you get `undefined reference to get_var_*` or `action_*`, you forgot step 6.

#### Rules & pitfalls

- **Never hand-edit `screens.c`, `styles.c`, `ui.c`, `eez-flow.*`, `images.c`.** Your edits will be clobbered on next EEZ export and create painful merge conflicts.
- **Keep `lv_conf.h` font list in sync** with fonts used in EEZ Studio. If you add a widget that needs `lv_font_montserrat_16`, enable `#define LV_FONT_MONTSERRAT_16 1` in `lv_conf.h` or the build will fail.
- **Object names matter.** `ParameterController` references `objects.obj7/obj8/obj9` and `objects.bias_*_node`. Renaming a widget ID in EEZ without updating `controller/ParameterController.cpp:kParams[]` will break compilation (or worse, silently bind the wrong label).
- **Compression off.** `eez-flow.h: EEZ_FOR_LVGL_LZ4_OPTION 0` — assets are uncompressed. Enabling LZ4 saves flash but requires `LV_USE_DRAW_SW_COMPLEX`.
- **Groups are encoder navigation.** Every focusable object must belong to exactly one `groups.*_grp`. Omitting a group assignment makes the object unreachable via the encoder.

---

## Firmware Architecture

```
Serial (USB 115200) ─┐
                     ├─► UARTPort / serial_input ─► RSProtocol::Protocol (CRC16)
RS-485 (SWSerial 9600)┘                               │ poll(MasterPacket)
                                                      ▼
                                            ParameterController::onMasterPacket
                                            (scales raw → milli-units telemetry)
                                                      │
Display (TFT_eSPI) ◄─ DisplayPort (lv_disp_flush) ◄─ vars.h get_var_*()
Encoder (EC11) ──────► DisplayPort (lv_indev read) ──► groups/main_grp/edit_grp
                                                      ▼
                                            ParameterController::sendState
                                            (SlavePacket → RS485 TX)
```

---

## Build / Flash / Monitor

```powershell
pio run -e rp2040                       # build only
pio run -e rp2040 --target upload       # build + upload (uses upload_port = COM18)
pio device monitor --baud 115200        # serial monitor (displays PROTOCOL_DEBUG hex dumps)

# Change upload port ad-hoc:
pio run -e rp2040 --target upload --upload-port COM5

# Clean build cache:
pio run --target clean
Remove-Item -Recurse -Force .pio
```

**TFT_eSPI wiring** is fixed in `include/User_Setup.h` (ILI9341, SPI0 @ 40 MHz):

| Signal | GPIO |
|---|---|
| MISO | 5 |
| MOSI | 6 |
| SCLK | 7 |
| CS   | 4 |
| DC   | 14 |
| RST  | 15 |

Change these and `Config.h:ENC_*_PIN` / `RS48_*` if you rewire the harness, then rebuild.

---

## Semantic Versioning History

This project had no git tags; versions below are **retrospective SemVer** reconstructed from `git log --all --oneline --reverse`. They are proposed as the basis for future tagging (`git tag v0.8.0`).

We follow **SemVer 2.0.0**: `MAJOR.MINOR.PATCH` — MAJOR = breaking wire-format or hardware incompatibility, MINOR = backward-compatible feature, PATCH = fix.

| Version | Date | Commit(s) | Type | Notes |
|---|---|---|---|---|
| **0.1.0** | 2026-06-24 | `8d1bef9` | init | Repo scaffold: `platformio.ini`, `src/main.cpp` (LVGL demo stub), `lib/README`. |
| **0.2.0** | 2026-06-26 | `efae1c6`, `db3224c`, `84cab72` | feat | LVGL 8.4 + TFT_eSPI bring-up. `lv_conf.h`, first EEZ Studio export (`screens.c`, `styles.c`, `eez-flow.*`, `ui.*`, `vars.h`, `images.*`). Display flush + draw buffer. |
| **0.2.1** | 2026-06-28 – 06-29 | `ffe7f0f`, `af73678` | fix | Splash→Main navigation, `ui_init` wiring, LVGL rotation/tick fixes. |
| **0.3.0** | 2026-06-30 | `dc70bac` | feat | First protocol layer: `lib/protocol/protocol.h` (generic `Packet`), `Config.h` v1 (bias/dither tunables), `event_types.h`, `events/serial_input` skeleton, `platformio.ini` adds `RotaryEncoder/OneButton/NeoPixel`. |
| **0.4.0** | 2026-07-07 | `958ee9b` | feat | Core firmware skeleton: `App` orchestrator, `RotaryInput`, `system/DisplayPort`, `system/UARTPort`, display+encoder LVGL drivers, CQST/laser image assets. |
| **0.5.0** | 2026-08-24 | `3363598` | feat | Theme support (`ThemeController`, `THEME_ID_DEFAULT/DARK_THEME`, `theme_colors`), `RSProtocol` v1 (`protocol.h` stub), `Actions.h` app enums, `User_Setup.h` SPI pinout refined, `lv_conf.h` widget set trimmed. |
| **0.6.0** | 2026-08-25 | `e480e16` | feat | **Parameter editing**: `ParameterController` (spinbox editors, `main_grp↔edit_grp` swapping, `kParams` table), `RSProtocol` Master/Slave packet framing, `RSPort` NeoPixel helpers, `RotaryInput` polish. |
| **0.6.1** | 2026-09-10 | `2cae6b1` | feat | `User_Setup.h` finalized (TFT 240×320 @ 40 MHz), `MasterPort` introduced (later renamed `RSPort`), `App::setup` simplified, serial input scaling groundwork. |
| **0.7.0** | 2026-09-10 | `db02647` | feat/breaking | **Wire-format stabilization:** `MasterPacket`/`SlavePacket` canonical structs (`header 0xFC/footer 0x23`), `RSPort→Serial` abstraction fix, `MasterPort` removed in favor of unified `RSPort + RSProtocol::Protocol` (SoftwareSerial + CRC16), `vars.h` telemetry bindings completed. |
| **0.7.1** | 2026-09-11 | `972f680` | fix | Bias Point made write-only (slave → master only); prevents master echo from clobbering spinbox. |
| **0.7.2** | 2026-09-11 | `e7f7e85` | fix | `RSProtocol` payload size corrections (`PAYLOAD_SIZE 18`, `MASTER_PAYLOAD_SIZE 20`), `Config.h` range tweaks. |
| **0.7.3** | 2026-09-11 | `ae8487b` | fix | `get16/put16` endian + CRC16 fixes, `plan.md`/`worklog.md` removed. |
| **0.7.4** | 2026-09-11 | `5293109` | docs/fix | `lib/RSProtocol/README.md` added (packet spec), `protocol.h` header/footer constants corrected. |
| **0.8.0** | 2026-09-16 | `27415c9`, `904eda4` | feat | EEZ Studio UI polish: `screens.c` layout/flex tweaks, `styles.c` + `styles.h` (new `screen_bg` style variants), `Config.h` dither/temperature unit refinements, `ParameterController` display-scale fixes. |
| **0.8.1** | 2026-09-19 | `README + clean` | docs/chore | **This release.** Comprehensive `README.md` (structure, Config.h, eez-flow pipeline, SemVer history) + project cleanup: removed `compile_commands.json` (now git-ignored), PlatformIO scaffolding `README`s, `lovyanGfxSetup.h`, dead `system/UI` shim, unused `ToggleController`, `.zcode` scratch plans; hardened `.gitignore` (`.pio/`, `compile_commands.json`, IDE user files); patched `App.cpp`/`DisplayPort.cpp` dead includes. |
| **1.0.0** | *TBD* | — | — | Planned first stable: hardware validated on production PCB, wire-format frozen, calibration flow E2E tested, tags + CI. |

#### How to tag going forward

```powershell
# v0.8.1 was tagged with this README + cleanup:
git tag -a v0.8.1 -m "docs: README + chore: clean project (remove dead code, harden .gitignore)"
git push origin v0.8.1

# For next feature (e.g., new telemetry field):
#  → bump MINOR to v0.9.0
# For wire-format breaking change:
#  → bump MAJOR to v1.0.0 or v0.8→v0.9 with BREAKING CHANGE note
```

---

## License

MIT (eez-framework portion) — see `src/ui/eez-flow.h` header.  
Firmware (`src/`, `include/`, `lib/RSProtocol/`) — all rights reserved by the authors (Iman Mokhtariaski) unless otherwise noted. Add a `LICENSE` file at the repo root if you intend to open-source the application layer.

---

## Acknowledgements

- [LVGL](https://lvgl.io) — embedded graphics
- [EEZ Studio / Envox](https://www.envox.eu/eez-studio/) — visual UI + flow codegen
- [Bodmer TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) — ILI9341 driver
- [Paul Stoffregen XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen)
- [mathertel RotaryEncoder / OneButton](https://github.com/mathertel/RotaryEncoder)

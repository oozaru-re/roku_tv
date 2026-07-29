#pragma once

#include <gui/icon.h>
#include <infrared.h>
#include <stdint.h>

typedef enum {
    ButtonPower,

    ButtonHome,
    ButtonGuide,
    ButtonReturn,

    ButtonUp,
    ButtonLeft,
    ButtonOk,
    ButtonRight,
    ButtonDown,

    ButtonVolumeUp,
    ButtonChannelUp,
    ButtonVolumeDown,
    ButtonChannelDown,

    ButtonNetflix,
    ButtonDisney,
    ButtonPrimeVideo,
    ButtonAppleTv,

    ButtonMax,
} TvButton;

typedef struct {
    const char* label;
    const Icon* icon;
    const Icon* selected_icon;

    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;

    TvButton nav_up;
    TvButton nav_down;
    TvButton nav_left;
    TvButton nav_right;
} RemoteButtonDef;

typedef struct {
    InfraredProtocol protocol;
    uint32_t address;
    uint32_t command;
} RemoteIrCode;

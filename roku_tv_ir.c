#include "roku_tv_ir.h"

#include <furi.h>
#include <furi_hal_infrared.h>

#include <infrared_signal.h>

#define ROKU_IR_ADDRESS 0xC7EAU

static const RemoteIrCode roku_tv_ir_codes[ButtonMax] = {
    [ButtonPower] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xE817U,
    },

    [ButtonHome] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xFC03U,
    },
    [ButtonGuide] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xF609U,
    },
    [ButtonReturn] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0x9966U,
    },

    [ButtonUp] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xE619U,
    },
    [ButtonLeft] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xE11EU,
    },
    [ButtonOk] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xD52AU,
    },
    [ButtonRight] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xD22DU,
    },
    [ButtonDown] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xCC33U,
    },

    [ButtonVolumeUp] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xF00FU,
    },
    [ButtonChannelUp] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xE41BU,
    },
    [ButtonVolumeDown] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xEF10U,
    },
    [ButtonChannelDown] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xE31CU,
    },

    [ButtonNetflix] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xAD52U,
    },
    [ButtonDisney] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xF30CU,
    },
    [ButtonPrimeVideo] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0xB44BU,
    },
    [ButtonAppleTv] = {
        .protocol = InfraredProtocolNECext,
        .address = ROKU_IR_ADDRESS,
        .command = 0x8877U,
    },
};

void roku_tv_ir_send(TvButton button) {
    furi_assert(button < ButtonMax);

    const RemoteIrCode* code = &roku_tv_ir_codes[button];

    InfraredMessage message = {
        .protocol = code->protocol,
        .address = code->address,
        .command = code->command,
        .repeat = false,
    };

    furi_hal_infrared_set_tx_output(FuriHalInfraredTxPinInternal);

    InfraredSignal* signal = infrared_signal_alloc();
    furi_check(signal);

    infrared_signal_set_message(signal, &message);
    infrared_signal_transmit(signal);
    infrared_signal_free(signal);
}

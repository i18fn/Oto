#include <oto.h>
#include <oto_sound.h>

#include "vm.h"

void oto_instr_print() {
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        printf("%f\n", vmstack_popv()->value.f);
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        printf("%f\n", vmstack_popf());
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        oto_error_exit(OTO_MISSING_ARGUMENTS_ERROR);
    }
}

void oto_instr_beep() {
    double duration = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        duration = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        duration = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        duration = 1;
    }

    double freq = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        freq = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        freq = 500.0;
    }

    Beep(freq, duration * 1000);
}

void oto_instr_play() {
    double duration = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        duration = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        duration = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        duration = 1;
    }

    double freq = 0;
    if (vmstack_typecheck() == VM_TY_VARPTR) {
        freq = vmstack_popv()->value.f;
    } else if (vmstack_typecheck() == VM_TY_IMMEDIATE) {
        freq = vmstack_popf();
    } else if (vmstack_typecheck() == VM_TY_INITVAL) {
        freq = 500.0;
    }

    Playdata data;
    data.freq[0] = freq;
    data.length  = 44100 * duration;
    data.sound   = NULL;
    data.volume  = 100;

    start_sound_stream();
    update_out_data(data);

    while (get_stream_status()) {
        // Pa_Sleep(1);
    }
    
    stop_sound_stream();
}
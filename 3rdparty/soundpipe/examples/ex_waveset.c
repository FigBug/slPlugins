#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_waveset *waveset;
    sp_diskin *diskin;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT wav = 0, waveset = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &wav);
    sp_waveset_compute(sp, ud->waveset, &wav, &waveset);
    sp_out(sp, 0, waveset);
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_waveset_create(&ud.waveset);
    sp_diskin_create(&ud.diskin);

    sp_waveset_init(sp, ud.waveset, 1.0);
    ud.waveset->rep = 3.0;
    sp_diskin_init(sp, ud.diskin, "oneart.wav");

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_waveset_destroy(&ud.waveset);
    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_compressor *compressor;
    sp_diskin *diskin;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT diskin = 0, compressor = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    sp_compressor_compute(sp, ud->compressor, &diskin, &compressor);
    sp->out[0] = compressor;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_compressor_create(&ud.compressor);
    sp_diskin_create(&ud.diskin);

    sp_compressor_init(sp, ud.compressor);
    *ud.compressor->ratio = 4;
    *ud.compressor->thresh = -30;
    *ud.compressor->atk = 0.2;
    *ud.compressor->rel = 0.2;
    sp_diskin_init(sp, ud.diskin, "oneart.wav");

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_compressor_destroy(&ud.compressor);
    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}

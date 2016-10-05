#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_bitcrush *bitcrush;
    sp_diskin *file;
    sp_ftbl *ft;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT disk = 0, bitcrush = 0;
    sp_diskin_compute(sp, ud->file, NULL, &disk);
    sp_bitcrush_compute(sp, ud->bitcrush, &disk, &bitcrush);
    sp->out[0] = bitcrush;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_bitcrush_create(&ud.bitcrush);
    sp_diskin_create(&ud.file);

    sp_diskin_init(sp, ud.file, "oneart.wav");
    sp_bitcrush_init(sp, ud.bitcrush);
    ud.bitcrush->bitdepth = 8;
    ud.bitcrush->srate = 10000;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_bitcrush_destroy(&ud.bitcrush);
    sp_diskin_destroy(&ud.file);

    sp_destroy(&sp);
    return 0;
}

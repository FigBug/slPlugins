#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_progress *progress;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, progress = 0;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp_progress_compute(sp, ud->progress, &osc, &progress);
    sp->out[0] = progress;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp->len = 44100 * 30;

    sp_progress_create(&ud.progress);
    sp_osc_create(&ud.osc);

    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_progress_init(sp, ud.progress);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp_process(sp, &ud, process);

    sp_progress_destroy(&ud.progress);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    sp_destroy(&sp);
    return 0;
}

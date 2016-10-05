#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_fof *fof;
    sp_ftbl *sine;
    sp_ftbl *win;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, fof = 0;
    sp_fof_compute(sp, ud->fof, &osc, &fof);
    sp->out[0] = fof;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_ftbl_create(sp, &ud.sine, 2048);
    sp_ftbl_create(sp, &ud.win, 1024);
    sp_fof_create(&ud.fof);

    sp_gen_sine(sp, ud.sine);
    sp_gen_composite(sp, ud.win, "0.5 0.5 270 0.5");

    sp_fof_init(sp, ud.fof, ud.sine, ud.win, 100, 0);

    sp->len = 44100 * 10;
    sp_process(sp, &ud, process);

    sp_fof_destroy(&ud.fof);
    sp_ftbl_destroy(&ud.sine);
    sp_ftbl_destroy(&ud.win);

    sp_destroy(&sp);
    return 0;
}

/*
 * This module is similar to "in", execpt that it will only read from STDIN
 * when triggered.
 *
 * This program expects to read SPFLOATs from STDIN, which control
 * the oscillator frequency in Hz. Here is one usage case, using Sporth
 * (paulbatchelor.github.io/proj/sporth):
 *
 * echo "0.5 noise 0.5 + 100 * 1000 +" | sporth -o raw -d 10 | ./ex_tin.bin
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_tin *in;
    sp_osc *osc;
    sp_ftbl *ft;
    sp_metro *met;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT in = 0, met = 0, osc = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    sp_tin_compute(sp, ud->in, &met, &in);
    ud->osc->freq = in;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_tin_create(&ud.in);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 4096);
    sp_metro_create(&ud.met);

    sp_tin_init(sp, ud.in);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 2;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_tin_destroy(&ud.in);
    sp_metro_destroy(&ud.met);
    sp_osc_destroy(&ud.osc);
    sp_ftbl_destroy(&ud.ft);

    sp_destroy(&sp);
    return 0;
}

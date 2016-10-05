#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc[3];
    sp_ftbl *ft;
    sp_lpf18 *lpf;
    sp_osc *lfo;
    sp_ftbl *sine_ft;
} UserData;

void write_osc(sp_data *sp, void *udata) {
    UserData *ud = udata;
    int i;
    SPFLOAT out = 0, osc = 0, filt = 0, lfo = 0;
    for(i = 0; i < 3; i++) {
       sp_osc_compute(sp, ud->osc[i], NULL, &osc);
       out += osc * 0.3;
    }
    sp_osc_compute(sp, ud->lfo, NULL, &lfo);
    ud->lpf->cutoff = 100 + 3000 * (0.5 * (lfo + 1));
    sp_lpf18_compute(sp, ud->lpf, &out, &filt);
    sp->out[0] = filt;
}

int main() {
    srand(time(NULL));
    int i;
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 4096);
    for(i = 0; i < 3; i++) sp_osc_create(&ud.osc[i]);
    sp_lpf18_create(&ud.lpf);
    sp_osc_create(&ud.lfo);
    sp_ftbl_create(sp, &ud.sine_ft, 4096);
    sp_gen_sine(sp, ud.sine_ft);

    sp_gen_line(sp, ud.ft, "0 1 4096 -1");
    sp_osc_init(sp, ud.osc[0], ud.ft, 0);
    ud.osc[0]->freq = sp_midi2cps(40 - 0.05);
    sp_osc_init(sp, ud.osc[1], ud.ft, 0);
    ud.osc[1]->freq = sp_midi2cps(40 - 12);
    sp_osc_init(sp, ud.osc[2], ud.ft, 0);
    ud.osc[2]->freq = sp_midi2cps(40 + 0.05);
    sp_lpf18_init(sp, ud.lpf);
    sp_osc_init(sp, ud.lfo, ud.sine_ft, 0);
    ud.lfo->freq = 0.4;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_ftbl_destroy(&ud.ft);
    for(i = 0; i < 3; i++) sp_osc_destroy(&ud.osc[i]);
    sp_lpf18_destroy(&ud.lpf);
    sp_osc_destroy(&ud.lfo);
    sp_ftbl_destroy(&ud.sine_ft);

    sp_destroy(&sp);
    return 0;
}

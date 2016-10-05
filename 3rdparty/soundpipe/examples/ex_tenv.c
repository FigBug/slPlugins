#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft;
    sp_metro *met;
    sp_tenv *tenv;
    SPFLOAT freq;
} UserData;

void write_osc(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT trig = 0;
    SPFLOAT env = 0;
    SPFLOAT osc = 0;
    sp_metro_compute(sp, ud->met, NULL, &trig);
    if(trig) ud->osc->freq = 500 + sp_rand(sp) % 2000;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp_tenv_compute(sp, ud->tenv, &trig, &env);

    sp->out[0] = osc * env;
}

int main() {
    UserData ud;
    SPFLOAT *freqp = &ud.freq;
    ud.freq = 400;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 123456);

    sp_tenv_create(&ud.tenv);
    sp_metro_create(&ud.met);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.03;
    ud.tenv->hold = 0.1;
    ud.tenv->rel = 0.1;
    sp_metro_init(sp, ud.met);
    ud.met->freq = 3;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = *freqp;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_tenv_destroy(&ud.tenv);
    sp_metro_destroy(&ud.met);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_destroy(&sp);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc[3];
    sp_ftbl *ft;
    sp_moogladder *filt;
    sp_bal *bal;
    sp_tenv *env;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    int i;
    SPFLOAT out = 0, osc = 0, filt = 0, bal = 0, env = 0, tick;
    for(i = 0; i < 3; i++) {
       sp_osc_compute(sp, ud->osc[i], NULL, &osc);
       out += osc * 0.5;
    }
    tick = (sp->pos == 0) ? 1.0 : 0.0;
    sp_tenv_compute(sp, ud->env, &tick, &env);
    ud->filt->freq = 300 + 3000 * env;
    sp_moogladder_compute(sp, ud->filt, &out, &filt);
    sp_bal_compute(sp, ud->bal, &filt, &osc, &bal);
    sp->out[0] = bal * env;
}

int main() {
    srand(time(NULL));
    int i;
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 4096);
    for(i = 0; i < 3; i++) {
        sp_osc_create(&ud.osc[i]);
        ud.osc[i]->amp = 0.5;
    }
    sp_bal_create(&ud.bal);
    sp_moogladder_create(&ud.filt);
    ud.filt->res = 0.8;
    sp_tenv_create(&ud.env);

    sp_gen_line(sp, ud.ft, "0 1 4096 -1");
    sp_osc_init(sp, ud.osc[0], ud.ft, 0);
    ud.osc[0]->freq = sp_midi2cps(41 - 0.05);
    sp_osc_init(sp, ud.osc[1], ud.ft, 0);
    ud.osc[1]->freq = sp_midi2cps(41 - 12);
    sp_osc_init(sp, ud.osc[2], ud.ft, 0);
    ud.osc[2]->freq = sp_midi2cps(41 + 0.05);
    sp->len = 44100 * 5;
    sp_bal_init(sp, ud.bal);
    sp_moogladder_init(sp, ud.filt);
    sp_tenv_init(sp, ud.env);
    ud.env->atk = 2.25;
    ud.env->hold = 0.5;
    ud.env->rel =  2.25;


    sp_process(sp, &ud, process);

    sp_ftbl_destroy(&ud.ft);
    for(i = 0; i < 3; i++) sp_osc_destroy(&ud.osc[i]);
    sp_bal_destroy(&ud.bal);
    sp_moogladder_destroy(&ud.filt);
    sp_tenv_destroy(&ud.env);
    sp_destroy(&sp);
    return 0;
}

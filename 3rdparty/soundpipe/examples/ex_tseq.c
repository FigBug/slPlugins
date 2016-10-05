#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft, *delta, *nn;
    sp_tenv *tenv;
    sp_dtrig *dt;
    sp_tseq *seq;
} UserData;

void write_osc(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT env = 0;
    SPFLOAT osc = 0;
    SPFLOAT dry = 0;
    SPFLOAT trig;
    SPFLOAT dtrig = 0;
    SPFLOAT note = 0;
    if(sp->pos == 0){
        trig = 1.0;
    }else{
        trig = 0.0;
    }
    sp_dtrig_compute(sp, ud->dt, &trig, &dtrig);
    sp_tseq_compute(sp, ud->seq, &dtrig, &note);
    ud->osc->freq = sp_midi2cps(note + 24);
    env = 1.0;
    sp_tenv_compute(sp, ud->tenv, &dtrig, &env);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc * env;
}

int main() {
    srand(time(NULL));
    UserData ud;
    uint32_t bufsize;
    sp_data *sp;
    sp_create(&sp);
    sp_auxdata rvaux;
    sp_dtrig_create(&ud.dt);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_ftbl_create(sp, &ud.delta, 4);
    sp_ftbl_create(sp, &ud.nn, 1);
    sp_osc_create(&ud.osc);

    sp_gen_vals(sp, ud.delta, "0.2 0.2 0.2 0.1");
    sp_gen_vals(sp, ud.nn, "60 62 64 67");
    sp_tseq_create(&ud.seq);
    sp_tseq_init(sp, ud.seq, ud.nn);

    sp_dtrig_init(sp, ud.dt, ud.delta);
    ud.dt->loop = 1.0;
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.01;
    ud.tenv->hold = 0.01;
    ud.tenv->rel =  0.05;

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 1000;
    ud.osc->amp = 0.5;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_dtrig_destroy(&ud.dt);
    sp_tseq_destroy(&ud.seq);
    sp_tenv_destroy(&ud.tenv);
    sp_ftbl_destroy(&ud.ft);
    sp_ftbl_destroy(&ud.nn);
    sp_ftbl_destroy(&ud.delta);
    sp_osc_destroy(&ud.osc);
    sp_destroy(&sp);
    return 0;
}

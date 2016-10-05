#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft;
    sp_tevent *te;
    sp_metro *met;
    sp_tenv *tenv;
    sp_maygate *mg;
    SPFLOAT freq;
} UserData;

void write_osc(sp_data *data, void *ud) {
    UserData *udp = ud;
    SPFLOAT trig = 0;
    SPFLOAT env = 0;
    SPFLOAT osc = 0;
    SPFLOAT mgate = 0;
    sp_metro_compute(data, udp->met, NULL, &trig);
    sp_maygate_compute(data, udp->mg, &trig, &mgate);
    sp_tevent_compute(data, udp->te, &trig, &udp->osc->freq);
    sp_tenv_compute(data, udp->tenv, &trig, &env);
    sp_osc_compute(data, udp->osc, NULL, &osc);
    data->out[0] = osc * env * mgate;
}

void freq_reinit(void *ud){
    SPFLOAT *freq = ud;
    *freq = 500 + rand() % 2000;
}

void freq_compute(void *ud, SPFLOAT *out){
    SPFLOAT *freq = ud;
    *out = *freq;
}

int main() {
    srand(time(NULL));
    UserData ud;
    SPFLOAT *freqp = &ud.freq;
    ud.freq = 400;
    sp_data *sp;
    sp_create(&sp);

    sp_maygate_create(&ud.mg);
    sp_tenv_create(&ud.tenv);
    sp_metro_create(&ud.met);
    sp_tevent_create(&ud.te);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_maygate_init(sp, ud.mg);
    ud.mg->prob = 0.2;
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.01;
    ud.tenv->hold = 0.01;
    ud.tenv->rel =  0.2;

    sp_metro_init(sp, ud.met);
    ud.met->freq = 12;
    sp_tevent_init(sp, ud.te, freq_reinit, freq_compute, freqp);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = *freqp;
    sp->len = 44100 * 5;

    sp_process(sp, &ud, write_osc);

    sp_maygate_destroy(&ud.mg);
    sp_tenv_destroy(&ud.tenv);
    sp_metro_destroy(&ud.met);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_tevent_destroy(&ud.te);
    sp_destroy(&sp);
    return 0;
}

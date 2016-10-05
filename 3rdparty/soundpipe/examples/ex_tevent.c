#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft; 
    sp_tevent *te;
    int counter;
    SPFLOAT freq;
} UserData;

void write_osc(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT trig = 0;
    if(ud->counter == 0){
        trig = 1.0;
    }
    sp_tevent_compute(sp, ud->te, &trig, &ud->osc->freq);
    sp_osc_compute(sp, ud->osc, NULL, &sp->out[0]);
    ud->counter = (ud->counter + 1) % 4410;
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
    ud.counter = 0;
    ud.freq = 400;
    sp_data *sp;
    sp_create(&sp);
    sp_tevent_create(&ud.te);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_tevent_init(sp, ud.te, freq_reinit, freq_compute, freqp);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = *freqp;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_tevent_destroy(&ud.te);
    sp_destroy(&sp);
    return 0;
}

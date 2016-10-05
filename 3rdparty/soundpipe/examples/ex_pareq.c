#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_pareq *pareq;
    sp_noise *noise;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT noise = 0, pareq = 0;
    sp_noise_compute(sp, ud->noise, NULL, &noise);
    sp_pareq_compute(sp, ud->pareq, &noise, &pareq);
    sp->out[0] = pareq;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_pareq_create(&ud.pareq);
    sp_noise_create(&ud.noise);

    sp_pareq_init(sp, ud.pareq);
    ud.pareq->fc = 500;
    sp_noise_init(sp, ud.noise);
    ud.noise->amp = 0.4;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_pareq_destroy(&ud.pareq);
    sp_noise_destroy(&ud.noise);

    sp_destroy(&sp);
    return 0;
}

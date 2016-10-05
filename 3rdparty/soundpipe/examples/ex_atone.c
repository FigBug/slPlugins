#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_atone *atone;
    sp_noise *noise;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT noise = 0, atone = 0;
    sp_noise_compute(sp, ud->noise, NULL, &noise);
    sp_atone_compute(sp, ud->atone, &noise, &atone);
    sp->out[0] = atone;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_atone_create(&ud.atone);
    sp_noise_create(&ud.noise);

    sp_atone_init(sp, ud.atone);
    sp_noise_init(sp, ud.noise);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_atone_destroy(&ud.atone);
    sp_noise_destroy(&ud.noise);

    sp_destroy(&sp);
    return 0;
}

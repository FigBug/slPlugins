#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_wpkorg35 *wpkorg35;
    sp_noise *noise;
    sp_randi *randi;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT noise = 0, wpkorg35 = 0, randi = 0;
    sp_randi_compute(sp, ud->randi, NULL, &randi);
    sp_noise_compute(sp, ud->noise, NULL, &noise);
    ud->wpkorg35->cutoff = randi;
    sp_wpkorg35_compute(sp, ud->wpkorg35, &noise, &wpkorg35);
    sp->out[0] = wpkorg35;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_wpkorg35_create(&ud.wpkorg35);
    sp_noise_create(&ud.noise);
    sp_randi_create(&ud.randi);

    sp_wpkorg35_init(sp, ud.wpkorg35);
    ud.wpkorg35->res = 1.99;
    sp_noise_init(sp, ud.noise);
    ud.noise->amp = 0.01;
    sp_randi_init(sp, ud.randi);
    ud.randi->min = 100;
    ud.randi->max = 3000;
    ud.randi->cps = 10;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_wpkorg35_destroy(&ud.wpkorg35);
    sp_noise_destroy(&ud.noise);
    sp_randi_destroy(&ud.randi);

    sp_destroy(&sp);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_pluck *pluck;
    sp_metro *met;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT pluck = 0, met = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    SPFLOAT notes[] = {60, 63, 67, 70, 74};
    if(met) {
        ud->pluck->freq = sp_midi2cps(notes[sp_rand(sp) % 5]);
    }
    sp_pluck_compute(sp, ud->pluck, &met, &pluck);
    sp->out[0] = pluck;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_pluck_create(&ud.pluck);
    sp_metro_create(&ud.met);

    sp_pluck_init(sp, ud.pluck, 400);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 4;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_pluck_destroy(&ud.pluck);
    sp_metro_destroy(&ud.met);

    sp_destroy(&sp);
    return 0;
}

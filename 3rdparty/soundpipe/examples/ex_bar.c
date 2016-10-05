#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_bar *bar;
    sp_metro *met;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT bar = 0, met = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    sp_bar_compute(sp, ud->bar, &met, &bar);
    sp->out[0] = bar;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_bar_create(&ud.bar);
    sp_metro_create(&ud.met);

    sp_bar_init(sp, ud.bar, 3, 0.0001);
    ud.bar->T30 = 1;

    sp_metro_init(sp, ud.met);
    ud.met->freq = 1;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_bar_destroy(&ud.bar);
    sp_metro_destroy(&ud.met);

    sp_destroy(&sp);
    return 0;
}

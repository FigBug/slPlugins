#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_metro *met;
    sp_nsmp *nsmp;
    sp_ftbl *ft;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT met = 0, nsmp = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    if(met) {
        ud->nsmp->index = rand() % 3;
    }
    sp_nsmp_compute(sp, ud->nsmp, &met, &nsmp);
    sp->out[0] = nsmp;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_ftbl_loadfile(sp, &ud.ft, "oneart.wav");
    sp_nsmp_create(&ud.nsmp);
    sp_metro_create(&ud.met);

    sp_metro_init(sp, ud.met);
    ud.met->freq = 2;
    sp_nsmp_init(sp, ud.nsmp, ud.ft, 44100, "oneart.ini");

    sp_nsmp_print_index(sp, ud.nsmp);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_nsmp_destroy(&ud.nsmp);
    sp_metro_destroy(&ud.met);

    sp_ftbl_destroy(&ud.ft);

    sp_destroy(&sp);
    return 0;
}

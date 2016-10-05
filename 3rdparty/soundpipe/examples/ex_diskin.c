#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_diskin *diskin;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT diskin = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    sp->out[0] = diskin;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_diskin_create(&ud.diskin);

    sp_diskin_init(sp, ud.diskin, "oneart.wav");

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}

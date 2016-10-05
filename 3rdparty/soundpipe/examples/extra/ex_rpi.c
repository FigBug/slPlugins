#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft; 
    int counter;
} udata;

void write_osc(sp_data *data, void *ud) {
    udata *udp = ud;
    if(udp->counter == 0){
        udp->osc->freq = 500 + rand() % 2000;
    }
    sp_osc_compute(data, udp->osc, NULL, &data->out[0]);
    udp->counter = (udp->counter + 1) % 4410;
}

int main() {
    srand(time(NULL));
    udata ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_gen_sine(ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 500;
    sp->len = 44100 * 5;
    sp_rpi_process(sp, &ud, write_osc);

    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_destroy(&sp);
    return 0;
}

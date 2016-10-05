#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft; 
    int counter;
} UserData;

void write_osc(sp_data *sp, void *udata) {
    UserData *ud = udata;
    //if(udp->counter == 0){
    //    udp->osc->freq = 500 + rand() % 2000;
    //}
    sp_osc_compute(sp, ud->osc, NULL, &sp->out[0]);
    ud->counter = (ud->counter + 1) % 4410;
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);
    
    sp_gen_file(sp, ud.ft, "Formant04.wav");
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 440;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_destroy(&sp);
    return 0;
}

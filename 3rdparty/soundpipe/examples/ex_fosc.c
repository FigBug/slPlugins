#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft; 
    int counter;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = ud;
    if(ud->counter == 0){
        ud->osc->freq = 500 + rand() % 2000;
    }
    sp_fosc_compute(sp, ud->osc, NULL, &sp->out[0]);
    ud->counter = (ud->counter + 1) % 4410;
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);

    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 500;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);
    sp_destroy(&sp);
    return 0;
}

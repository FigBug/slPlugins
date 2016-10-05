#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc1, *osc2;
    sp_ftbl *ft;
    int counter1, counter2;
} UserData;

void write_osc(sp_data *sp, void *udata) {
    UserData *ud = udata;
    if(ud->counter1 == 0){
        ud->osc1->freq = 500 + rand() % 2000;
    }
    if(ud->counter2 == 0){
        ud->osc2->freq = 500 + rand() % 2000;
    }
    sp_osc_compute(sp, ud->osc1, NULL, &sp->out[0]);
    sp_osc_compute(sp, ud->osc2, NULL, &sp->out[1]);
    ud->counter1 = (ud->counter1 + 1) % 4410;
    ud->counter2 = (ud->counter2 + 1) % 8000;
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter1 = 0;
    ud.counter2 = 0;
    sp_data *sp;
    /* two channels will write 0_test.wav and 1_test.wav */
    sp_createn(&sp, 2);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc1);
    sp_osc_create(&ud.osc2);

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc1, ud.ft, 0);
    ud.osc1->freq = 500;
    sp_osc_init(sp, ud.osc2, ud.ft, 0);
    ud.osc2->freq = 500;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc1);
    sp_osc_destroy(&ud.osc2);
    sp_destroy(&sp);
    return 0;
}

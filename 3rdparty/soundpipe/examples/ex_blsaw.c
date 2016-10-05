#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_blsaw *blsaw;
    int counter;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    if(ud->counter == 0){
        *ud->blsaw->freq = 500 + rand() % 2000;
    }
    sp_blsaw_compute(sp, ud->blsaw, NULL, &sp->out[0]);
    ud->counter = (ud->counter + 1) % 4410;
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);

    sp_blsaw_create(&ud.blsaw);

    sp_blsaw_init(sp, ud.blsaw);
    *ud.blsaw->freq = 500;
    *ud.blsaw->amp = 1;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_blsaw_destroy(&ud.blsaw);
    sp_destroy(&sp);
    return 0;
}

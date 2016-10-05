#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_blsquare *blsquare;
    int counter;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    if(ud->counter == 0){
        *ud->blsquare->freq = 500 + rand() % 2000;
    }
    sp_blsquare_compute(sp, ud->blsquare, NULL, &sp->out[0]);
    ud->counter = (ud->counter + 1) % 4410;
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);

    sp_blsquare_create(&ud.blsquare);

    sp_blsquare_init(sp, ud.blsquare);
    *ud.blsquare->freq = 500.0;
    *ud.blsquare->amp = 0.9;
    *ud.blsquare->width = 0.5;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_blsquare_destroy(&ud.blsquare);
    sp_destroy(&sp);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_bltriangle *bltriangle;
    int counter;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    if(ud->counter == 0){
        *ud->bltriangle->freq = 500 + rand() % 2000;
    }
    sp_bltriangle_compute(sp, ud->bltriangle, NULL, &sp->out[0]);
    ud->counter = (ud->counter + 1) % 4410;
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);

    sp_bltriangle_create(&ud.bltriangle);

    sp_bltriangle_init(sp, ud.bltriangle);
    *ud.bltriangle->freq = 500.0;
    *ud.bltriangle->amp = 0.9;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_bltriangle_destroy(&ud.bltriangle);
    sp_destroy(&sp);
    return 0;
}

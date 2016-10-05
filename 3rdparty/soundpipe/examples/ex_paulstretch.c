#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_paulstretch *paulstretch;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT paulstretch = 0;
    sp_paulstretch_compute(sp, ud->paulstretch, NULL, &paulstretch);
    sp->out[0] = paulstretch;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_paulstretch_create(&ud.paulstretch);
    sp_ftbl_loadfile(sp, &ud.ft, "oneart.wav");

    sp_paulstretch_init(sp, ud.paulstretch, ud.ft, 1.0, 10);

    sp->len = 44100 * 20;
    sp_process(sp, &ud, process);

    sp_paulstretch_destroy(&ud.paulstretch);
    sp_ftbl_destroy(&ud.ft);

    sp_destroy(&sp);
    return 0;
}

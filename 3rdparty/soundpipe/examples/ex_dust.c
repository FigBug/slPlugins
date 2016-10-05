#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_dust *dst;
} UserData;

void write_dust(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT in = 0;
    sp_dust_compute(sp, ud->dst, NULL, &sp->out[0]);
}
int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_dust_create(&ud.dst);
    sp_dust_init(sp, ud.dst);
    ud.dst->amp = 0.3;
    ud.dst->density = 30;
    sp->len = 44100 * 5;
    ud.dst->bipolar = 1.0;
    sp_process(sp, &ud, write_dust);
    sp_dust_destroy(&ud.dst);
    sp_destroy(&sp);
    return 0;
}

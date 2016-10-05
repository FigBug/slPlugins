#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_phasor *phs;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    sp_phasor_compute(sp, ud->phs, NULL, &sp->out[0]);
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_phasor_create(&ud.phs);

    sp_phasor_init(sp, ud.phs, 0);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_phasor_destroy(&ud.phs);
    sp_destroy(&sp);
    return 0;
}

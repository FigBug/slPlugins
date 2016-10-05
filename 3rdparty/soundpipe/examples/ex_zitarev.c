#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_diskin *disk;
    sp_zitarev *rev;
    int counter;
} UserData;

void process(sp_data *sp , void *udata) {
    UserData *ud = udata;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    SPFLOAT foo = 0;

    sp_diskin_compute(sp, ud->disk, NULL, &in);
    sp_zitarev_compute(sp, ud->rev, &in, &in, &sp->out[0], &foo);
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);
    sp_diskin_create(&ud.disk);
    sp_zitarev_create(&ud.rev);

    sp_diskin_init(sp, ud.disk, "oneart.wav");
    sp_zitarev_init(sp, ud.rev);
    *ud.rev->level = 0;
    *ud.rev->in_delay = 20;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_diskin_destroy(&ud.disk);
    sp_zitarev_destroy(&ud.rev);
    sp_destroy(&sp);
    return 0;
}

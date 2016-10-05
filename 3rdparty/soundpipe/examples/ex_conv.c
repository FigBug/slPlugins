#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_diskin *diskin;
    sp_conv *conv;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT conv = 0, diskin = 0, bal = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    sp_conv_compute(sp, ud->conv, &diskin, &conv);
    sp->out[0] = conv * 0.05;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_diskin_create(&ud.diskin);
    sp_conv_create(&ud.conv);
    sp_ftbl_loadfile(sp, &ud.ft, "imp.wav");

    sp_diskin_init(sp, ud.diskin, "oneart.wav");
    sp_conv_init(sp, ud.conv, ud.ft, 8192);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_conv_destroy(&ud.conv);
    sp_ftbl_destroy(&ud.ft);
    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}

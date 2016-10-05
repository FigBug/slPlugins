#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_autowah *autowah;
    sp_diskin *disk;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT disk = 0, autowah = 0;
    sp_diskin_compute(sp, ud->disk, NULL, &disk);
    sp_autowah_compute(sp, ud->autowah, &disk, &autowah);
    sp->out[0] = autowah;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_autowah_create(&ud.autowah);
    sp_diskin_create(&ud.disk);

    sp_diskin_init(sp, ud.disk, "riff.wav");
    sp_autowah_init(sp, ud.autowah);
    *ud.autowah->wah = 1.0;
    
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_autowah_destroy(&ud.autowah);
    sp_diskin_destroy(&ud.disk);

    sp_destroy(&sp);
    return 0;
}

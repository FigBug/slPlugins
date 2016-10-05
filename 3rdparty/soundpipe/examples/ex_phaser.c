#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_phaser *phaser;
    sp_diskin *disk;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT disk1 = 0, disk2 = 0, phaser = 0, foo = 0;
    sp_diskin_compute(sp, ud->disk, NULL, &disk1);
    disk2 = disk1;
    sp_phaser_compute(sp, ud->phaser, &disk1, &disk2, &phaser, &foo);
    sp->out[0] = phaser;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_phaser_create(&ud.phaser);
    sp_diskin_create(&ud.disk);

    sp_diskin_init(sp, ud.disk, "oneart.wav");
    sp_phaser_init(sp, ud.phaser);
    
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_phaser_destroy(&ud.phaser);
    sp_diskin_destroy(&ud.disk);

    sp_destroy(&sp);
    return 0;
}

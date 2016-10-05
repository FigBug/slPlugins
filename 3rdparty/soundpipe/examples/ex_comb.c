#include "soundpipe.h"

typedef struct {
    sp_comb *comb;
    sp_diskin *disk;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT comb = 0, disk = 0;
    sp_diskin_compute(sp, ud->disk, NULL, &disk);
    disk *= 0.5;
    sp_comb_compute(sp, ud->comb, &disk, &comb);

    sp->out[0] = comb;
}


int main()
{
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_comb_create(&ud.comb);

    sp_diskin_create(&ud.disk);

    sp_comb_init(sp, ud.comb, 0.01);
    sp_diskin_init(sp, ud.disk, "oneart.wav");
    
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_comb_destroy(&ud.comb);
    sp_diskin_destroy(&ud.disk);
    sp_destroy(&sp);
    return 0;
}

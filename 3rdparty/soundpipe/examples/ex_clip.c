#include <stdio.h>
#include "soundpipe.h"

typedef struct {
   sp_ftbl *ft;
   sp_osc *osc;
   sp_clip *clp;
   sp_moogladder *filt;
} UserData;

void write_clip(sp_data *sp, void *ud) {
    UserData *udp = ud;
    SPFLOAT out, osc, filt;
    sp_osc_compute(sp, udp->osc, NULL, &osc);
    sp_moogladder_compute(sp, udp->filt, &osc, &filt);
    sp_clip_compute(sp, udp->clp, &filt, &out);
    sp->out[0] = out;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_clip_create(&ud.clp);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 4096);
    sp_moogladder_create(&ud.filt);

    sp_gen_line(sp, ud.ft, "0 1 4096 -1");
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = sp_midi2cps(48);
    sp_clip_init(sp, ud.clp);
    sp_moogladder_init(sp, ud.filt);
    ud.filt->freq = 1000; 
    ud.filt->res = 0.8;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_clip);

    sp_osc_destroy(&ud.osc);
    sp_clip_destroy(&ud.clp);
    sp_ftbl_destroy(&ud.ft);
    sp_moogladder_destroy(&ud.filt);

    sp_destroy(&sp);
    return 0;
}

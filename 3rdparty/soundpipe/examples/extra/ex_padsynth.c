#include "soundpipe.h"


typedef struct user_data {
    sp_ftbl *ft, *amps;
    sp_osc *osc;
    SPFLOAT fc;
} user_data;

void process(sp_data *sp, void *userdata) {
    user_data *ud = userdata;
    //sp->out = ud->ft->tbl[sp->pos % ud->ft->size];
    sp_osc_compute(sp, ud->osc, NULL, &sp->out[0]);
}

int main() {
    user_data ud;
    int i;
    sp_data *sp;
    sp_create(&sp); sp_ftbl_create(sp, &ud.amps, 64);
    sp_ftbl_create(sp, &ud.ft, 262144);
    sp_osc_create(&ud.osc);

    sp->sr = 96000;
    sp->len = ud.ft->size;
    ud.amps->tbl[0] = 0.0;

    for(i = 1; i < ud.amps->size; i++){
        ud.amps->tbl[i] = 1.0 / i;
        if((i % 2) == 0) ud.amps->tbl[i] *= 2.0; 
    }

    
    /* Discovered empirically. multiply frequency by this constant. */
    ud.fc = 1 / (6.0 * 440); 
    sp_gen_padsynth(sp, ud.ft, ud.amps, 440, 40.0);

    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = sp_midi2cps(70) * ud.fc;
    ud.osc->amp = 1.0;
    sp_process(sp, &ud, process);

    sp_osc_destroy(&ud.osc);
    sp_ftbl_destroy(&ud.amps);
    sp_ftbl_destroy(&ud.ft);
    sp_destroy(&sp);
    return 0;
}

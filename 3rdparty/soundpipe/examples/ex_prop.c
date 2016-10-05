#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_prop *prop;
    sp_osc *osc;
    sp_ftbl *ft;
    sp_tenv *tenv;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, prop = 0, tenv = 0;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp_prop_compute(sp, ud->prop, NULL, &prop);
    sp_tenv_compute(sp, ud->tenv, &prop, &tenv);
    sp->out[0] = osc * tenv;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 123345);

    sp_prop_create(&ud.prop);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_tenv_create(&ud.tenv);

    sp_prop_init(sp, ud.prop, "2(+{3(+++)|+}){4(+?+?)|+|-}");
    ud.prop->bpm = 80;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.003;
    ud.tenv->hold = 0.001;
    ud.tenv->rel =  0.2;

    ud.osc->freq = 500;

    sp->len = 44100 * 20;
    sp_process(sp, &ud, process);

    sp_prop_destroy(&ud.prop);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_tenv_destroy(&ud.tenv);

    sp_destroy(&sp);
    return 0;
}

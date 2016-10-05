#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_line *line;
    sp_osc *osc;
    sp_ftbl *ft; 
    sp_dmetro *dm;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, line = 0, dm = 0;
    sp_dmetro_compute(sp, ud->dm, NULL, &dm);
    sp_line_compute(sp, ud->line, &dm, &line);
    ud->osc->freq = line;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_line_create(&ud.line);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_dmetro_create(&ud.dm);

    sp_line_init(sp, ud.line);
    ud.line->a = 100;
    ud.line->dur = 1;
    ud.line->b = 400;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_dmetro_init(sp, ud.dm);
    ud.dm->time = 2;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_line_destroy(&ud.line);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_dmetro_destroy(&ud.dm);

    sp_destroy(&sp);
    return 0;
}

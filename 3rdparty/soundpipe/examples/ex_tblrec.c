#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_tblrec *tblrec;
    sp_ftbl *ft; 
    sp_metro *met;
    sp_pluck *plk;
    sp_randi *randi;
    sp_tabread *tr;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT trig, pluck, rand, tr;
    SPFLOAT tick = (sp->pos == 0 ? 1 : 0);
    sp_metro_compute(sp, ud->met, NULL, &trig);
    sp_pluck_compute(sp, ud->plk, &trig, &pluck);
    sp_tblrec_compute(sp, ud->tblrec, &pluck, &tick, NULL);
    sp_randi_compute(sp, ud->randi, NULL, &rand);
    ud->tr->index = rand;
    sp_tabread_compute(sp, ud->tr, NULL, &tr);

    sp->out[0] = tr;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp->len = 44100 * 5;

    /* create */
    sp_tblrec_create(&ud.tblrec);
    sp_ftbl_create(sp, &ud.ft, sp->sr * 0.5);

    sp_metro_create(&ud.met);
    sp_pluck_create(&ud.plk); 
    sp_randi_create(&ud.randi);
    sp_tabread_create(&ud.tr);

    /* init */
    sp_tblrec_init(sp, ud.tblrec, ud.ft);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 2.5;
    sp_pluck_init(sp, ud.plk, 110);
    ud.plk->freq = 440;
    sp_randi_init(sp, ud.randi);
    sp_tabread_init(sp, ud.tr, ud.ft, 1);

    sp_process(sp, &ud, process);

    /* destroy */
    sp_tblrec_destroy(&ud.tblrec);
    sp_ftbl_destroy(&ud.ft);
    sp_metro_destroy(&ud.met);
    sp_pluck_destroy(&ud.plk); 
    sp_randi_destroy(&ud.randi);
    sp_tabread_destroy(&ud.tr);

    sp_destroy(&sp);
    return 0;
}

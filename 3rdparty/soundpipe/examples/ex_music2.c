#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_ftbl *ft, *delta;
    sp_dtrig *dt;
    sp_tevent *te;
    uint32_t pos;
} samp_data;

typedef struct {
    SPFLOAT tempo;
    sp_count *cnt;
    sp_ftbl *reps;
    sp_tseq *rpick;
    sp_metro *clk, *dblclk;
    sp_reverse *rvs;
    sp_maygate *rthrow, *rgate, *rvs_switch;
    sp_revsc *rev;
    sp_rpt *rpt;
    samp_data snare, kick, hh;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT dry;
    SPFLOAT kick, snare, hh, clk, dblclk, bar, dtrig, count;
    SPFLOAT revin, revout, rthrow, rgate, rpt, rvs, rvs_switch, foo;
    SPFLOAT reps;
    sp_metro_compute(sp, ud->clk, NULL, &clk);
    sp_metro_compute(sp, ud->dblclk, NULL, &dblclk);
    sp_count_compute(sp, ud->cnt, &clk, &count);
    bar = (count == 0 && clk);
    sp_dtrig_compute(sp, ud->snare.dt, &bar, &dtrig);
    sp_tevent_compute(sp, ud->snare.te, &dtrig, &snare);

    sp_maygate_compute(sp, ud->rthrow, &dtrig, &rthrow);
    revin = snare * rthrow;
    sp_revsc_compute(sp, ud->rev, &revin, &revin, &revout, &foo);

    sp_dtrig_compute(sp, ud->kick.dt, &bar, &dtrig);
    sp_tevent_compute(sp, ud->kick.te, &dtrig, &kick);
    sp_dtrig_compute(sp, ud->hh.dt, &bar, &dtrig);
    sp_tevent_compute(sp, ud->hh.te, &dtrig, &hh);

    dry = snare + kick + hh * 0.5 + revout * 0.1;
    sp_maygate_compute(sp, ud->rgate, &dblclk, &rgate);
    sp_tseq_compute(sp, ud->rpick, &dblclk, &reps);

    sp_rpt_compute(sp, ud->rpt, &rgate, &dry, &rpt);
    sp_reverse_compute(sp, ud->rvs, &rpt, &rvs);
    sp_maygate_compute(sp, ud->rvs_switch, &clk, &rvs_switch);

    sp->out[0] = (rvs_switch) ? rvs : rpt;
}

void samp_reinit(void *ud) {
    samp_data *sd = ud;
    sd->pos = 0;
}

void samp_compute(void *ud, SPFLOAT *out) {
    samp_data *sd = ud;
    if(sd->pos < sd->ft->size){
        *out = sd->ft->tbl[sd->pos];
        sd->pos++;
    } else {
        *out = 0;
    }
}
void samp_create(sp_data *sp, samp_data *sd,
    size_t size, char *file, char *dtrig, SPFLOAT tempo, SPFLOAT delay) {
    int i;
    SPFLOAT tscale = 60.0 / tempo;
    sp_ftbl_create(sp, &sd->delta, 1);
    sp_gen_vals(sp, sd->delta, dtrig);
    sp_dtrig_create(&sd->dt);
    for(i = 0; i < sd->delta->size; i++){
        sd->delta->tbl[i] *= tscale;
    }
    sp_dtrig_init(sp, sd->dt, sd->delta);

    sd->dt->delay = delay * tscale;

    sp_ftbl_create(sp, &sd->ft, 5574);
    sp_gen_file(sp, sd->ft, file);

    sp_tevent_create(&sd->te);
    sp_tevent_init(sp, sd->te, samp_reinit, samp_compute, sd);

    sd->pos = 0;
}

void samp_destroy(samp_data *sd){
    sp_tevent_destroy(&sd->te);
    sp_ftbl_destroy(&sd->ft);
    sp_ftbl_destroy(&sd->delta);
    sp_dtrig_destroy(&sd->dt);
}


int main() {
    srand(time(NULL));
    sp_data *sp;
    UserData ud;
    sp_create(&sp);

    SPFLOAT tempo = 144;
    ud.tempo = tempo;

    /* Create */
    sp_count_create(&ud.cnt);
    sp_metro_create(&ud.clk);
    sp_metro_create(&ud.dblclk);
    samp_create(sp, &ud.snare, 5574, "snare.wav", "2 0.75 0.25", tempo, 1);
    samp_create(sp, &ud.kick, 7385, "kick.wav", "0.75 1.25 1 1", tempo, 0);
    samp_create(sp, &ud.hh, 4507, "hh.wav",
            "0.5 0.25 0.5 0.5 0.25 0.5 0.25 0.5 0.5 0.25",
            tempo, 0);
    sp_revsc_create(&ud.rev);
    sp_maygate_create(&ud.rthrow);
    sp_maygate_create(&ud.rgate);
    sp_rpt_create(&ud.rpt);
    sp_ftbl_create(sp, &ud.reps, 1);
    sp_tseq_create(&ud.rpick);
    sp_gen_vals(sp, ud.reps, "4 8 16 6");
    sp_tseq_init(sp, ud.rpick, ud.reps);
    ud.rpick->shuf = 1.0;
    sp_reverse_create(&ud.rvs);
    sp_maygate_create(&ud.rvs_switch);

    /* Init */
    sp->len = 44100 * (60.0 / tempo) * 32;
    sp_count_init(sp, ud.cnt);
    ud.cnt->count = 4;
    sp_metro_init(sp, ud.clk);
    ud.clk->freq = tempo / 60.0;
    sp_metro_init(sp, ud.dblclk);
    ud.dblclk->freq = tempo * 2 / 60.0;
    sp_revsc_init(sp, ud.rev);
    ud.rev->feedback = 0.9;
    ud.rev->lpfreq = 10000;
    sp_maygate_init(sp, ud.rthrow);
    ud.rthrow->prob = 0.5;
    sp_rpt_init(sp, ud.rpt, 1.0);
    //sp_rpt_set(ud.rpt, tempo, 8, 4);
    ud.rpt->bpm = tempo;
    ud.rpt->div = 8;
    ud.rpt->reps = 4;
    sp_maygate_init(sp, ud.rgate);
    ud.rgate->prob = 0.18;
    ud.rgate->mode = 1.0;
    sp_reverse_init(sp, ud.rvs, 60.0 / tempo);
    sp_maygate_init(sp, ud.rvs_switch);
    ud.rvs_switch->prob = 0.1;

    /* Process */
    sp_process(sp, &ud, process);

    /* Destroy */
    samp_destroy(&ud.snare);
    samp_destroy(&ud.kick);
    samp_destroy(&ud.hh);
    sp_revsc_destroy(&ud.rev);
    sp_maygate_destroy(&ud.rthrow);
    sp_maygate_destroy(&ud.rgate);
    sp_maygate_destroy(&ud.rvs_switch);
    sp_count_destroy(&ud.cnt);
    sp_metro_destroy(&ud.clk);
    sp_metro_destroy(&ud.dblclk);
    sp_rpt_destroy(&ud.rpt);
    sp_ftbl_destroy(&ud.reps);
    sp_tseq_destroy(&ud.rpick);
    sp_reverse_destroy(&ud.rvs);

    sp_destroy(&sp);
    return 0;
}

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <soundpipe.h>
#define NUMLINE 3


typedef struct {
    sp_mode *mode[4];
    SPFLOAT *mfreq[4];
    SPFLOAT *Q[4];
    SPFLOAT amp;
    SPFLOAT *freq;
    SPFLOAT lfreq;
} modal;

typedef struct {
    sp_dust *dust;
    sp_osc *osc;
    sp_tenv *env;
    sp_tevent *retrig;
    SPFLOAT amp;
} chord_grain;

typedef struct {
    sp_ftbl *sine;
    sp_ftbl *notes;
    SPFLOAT ampmax;
    chord_grain *grains;
} chord_cloud;

int modal_create(modal **md)
{
    *md = malloc(sizeof(modal));
    return SP_OK;
}

int modal_init(sp_data *sp, modal *md)
{
    int i;
    md->amp = 0.45;

    for(i = 0; i < 4; i++) {
        sp_mode_create(&md->mode[i]);
        sp_mode_init(sp, md->mode[i]);
        md->mfreq[i] = &md->mode[i]->freq;
        md->Q[i] = &md->mode[i]->q;
    }

    *md->mfreq[0] = 1000;
    *md->mfreq[1] = 3000;
    *md->Q[0] = 12;
    *md->Q[1] = 8;

    *md->mfreq[2] = 440;
    *md->mfreq[3] = *md->mfreq[2] * 2.01081;
    *md->Q[2] = 500;
    *md->Q[3] = 420;

    md->freq = md->mfreq[2];
    md->lfreq = *md->freq;
    return SP_OK;
}

int modal_compute(sp_data *sp, modal *md, SPFLOAT *in, SPFLOAT *out)
{
    SPFLOAT exc1, exc2, exc;
    SPFLOAT res1, res2, res;

    if(*md->freq != md->lfreq) {
        *md->mfreq[3] = *md->freq * 2.01081;
        md->lfreq = *md->freq;
    }

    sp_mode_compute(sp, md->mode[0], in, &exc1);
    sp_mode_compute(sp, md->mode[1], in, &exc2);
    exc = (exc1 + exc2) * 0.5;

    if(exc > md->amp) {
        exc = md->amp;
    } else if (exc < 0 ) {
        exc = 0;
    }

    sp_mode_compute(sp, md->mode[2], &exc, &res1);
    sp_mode_compute(sp, md->mode[3], &exc, &res2);
    res = (res1 + res2) * 0.5;


    *out = (exc + res) * md->amp;

    return SP_OK;
}

int modal_destroy(modal **md)
{
    int i;
    modal *mdp = *md;
    for(i = 0; i < 4; i++) {
        sp_mode_destroy(&mdp->mode[i]);
    }

    free(*md);
    return SP_OK;
}

static void cloud_compute(void *ud, SPFLOAT *out) {
    chord_cloud *grain = ud;
}

static void cloud_reinit(void *ud) {
    chord_grain *grain = ud;
    int rval = rand();
/*
    Figure out why this clicks
    grain->amp = 0.8 + fabs(0.2 * (SPFLOAT) (1.0 * rval / RAND_MAX));
*/
}


int chord_cloud_create(chord_cloud **cc)
{
    *cc = malloc(sizeof(chord_cloud));
    return SP_OK;
}

int chord_cloud_init(sp_data *sp, chord_cloud *cc, char *notes, SPFLOAT amp)
{
    int i;
    sp_osc *tmposc;
    sp_dust *tmpdust;
    sp_tenv *tmpenv;
    sp_tevent *tmpretrig;

    sp_ftbl_create(sp, &cc->sine, 4096);
    sp_gen_sinesum(sp, cc->sine, "1 0.5 0.3 0.25, 0.1625");
    sp_ftbl_create(sp, &cc->notes, 1);
    sp_gen_vals(sp, cc->notes, notes);
    cc->ampmax= amp;

    cc->grains= malloc(cc->notes->size * sizeof(chord_grain));

    for(i = 0; i < cc->notes->size; i++) {
        chord_grain *grain = &cc->grains[i];
        sp_osc_create(&grain->osc);
        sp_tenv_create(&grain->env);
        sp_dust_create(&grain->dust);
        sp_tevent_create(&grain->retrig);

        tmposc = grain->osc;
        tmpenv = grain->env;
        tmpdust = grain->dust;
        tmpretrig = grain->retrig;

        sp_dust_init(sp, tmpdust);
        tmpdust->amp = 1;
        tmpdust->density = 8;
        sp_osc_init(sp, tmposc, cc->sine, 0);
        tmposc->freq = sp_midi2cps(cc->notes->tbl[i]);
        tmposc->amp = 1.0;
        grain->amp = 1.0;
        sp_tenv_init(sp, tmpenv);
        tmpenv->atk = 0.01;
        tmpenv->hold = 0;
        tmpenv->rel =  0.03;

        sp_tevent_init(sp, tmpretrig, cloud_reinit, cloud_compute, grain);
    }

    return SP_OK;
}

int chord_cloud_compute(sp_data *sp, chord_cloud *cc, SPFLOAT *in, SPFLOAT *out)
{
    int i;
    SPFLOAT dust = 0, osc = 0, env = 0, tmp = 0, dummy;
    for(i = 0; i < cc->notes->size; i++) {
        chord_grain *grain = &cc->grains[i];
        sp_dust_compute(sp, grain->dust, NULL, &dust);
        sp_tenv_compute(sp, grain->env, &dust, &env);
        sp_osc_compute(sp, grain->osc, NULL, &osc);
        sp_tevent_compute(sp, grain->retrig, &dust, &dummy);
        tmp += osc * env * cc->ampmax * grain->amp;
    }
    *out = tmp;
    return SP_OK;
}

int chord_cloud_destroy(chord_cloud **cc)
{
    int i;
    chord_cloud *ccp = *cc;
    sp_dust **tmpdust;
    sp_ftbl_destroy(&ccp->sine);

    for(i = 0; i < ccp->notes->size; i++ ) {
        sp_dust_destroy(&ccp->grains[i].dust);
        sp_osc_destroy(&ccp->grains[i].osc);
        sp_tenv_destroy(&ccp->grains[i].env);
        sp_tevent_destroy(&ccp->grains[i].retrig);
    }
    free(ccp->grains);
    sp_ftbl_destroy(&ccp->notes);
    free(*cc);
    return SP_OK;
}


typedef struct {
    chord_cloud *cc;
    sp_randi *randi;
} CloudLine;

typedef struct {
    CloudLine line[NUMLINE];
    sp_revsc *rev;
    sp_metro *clk;
    sp_count *meter;
    sp_drip *drip;
    sp_vdelay *del;
    SPFLOAT pdel;
    sp_tenv *master;
    modal *mod;
    sp_ftbl *notes;
    sp_tseq *seq;
    sp_maygate *mg;
    sp_progress *prog;
} UserData;

void process(sp_data *sp, void *udata)
{
    UserData *ud = udata;
    int i;
    SPFLOAT line = 0, cloud = 0, tmp = 0, revin = 0, rev = 0, dummy, drip = 0;
    SPFLOAT count = 0;
    SPFLOAT bar = 0, clk = 0, delIn = 0, delOut = 0;
    SPFLOAT mode = 0;

    for(i = 0; i < NUMLINE; i++) {
        sp_randi_compute(sp, ud->line[i].randi, NULL, &line);
        chord_cloud_compute(sp, ud->line[i].cc, NULL, &cloud);
        tmp += cloud * line;
    }
    revin += tmp * 0.5;
    sp_metro_compute(sp, ud->clk, NULL, &clk);
    sp_count_compute(sp, ud->meter, &clk, &count);
    bar = (count == 0 && clk);
    sp_drip_compute(sp, ud->drip, &bar, &drip);
    drip *= 0.5;
    revin += drip * 0.1;

    if(sp->pos > 44100 * 3) {
        SPFLOAT mg = 0, nn = 0;
        sp_maygate_compute(sp, ud->mg, &clk, &mg);
        sp_tseq_compute(sp, ud->seq, &mg, &nn);
        *ud->mod->freq = sp_midi2cps(nn + 12);
        modal_compute(sp, ud->mod, &mg, &mode);
        mode *= 0.3;
        revin += mode * 0.8;
        delIn += mode * 0.5;
    }

    delIn += drip * 0.1 + ud->pdel * 0.6;
    sp_vdelay_compute(sp, ud->del, &delIn, &delOut);
    ud->pdel = delOut;
    revin += delOut * 0.1;


    sp_revsc_compute(sp, ud->rev, &revin, &revin, &rev, &dummy);
    sp->out[0] = tmp * 0.2 + rev * 0.6 + drip * 0.1 + delOut * 0.5 + mode;

    /* Master fader */
    SPFLOAT mtick = (sp->pos == 0) ? 1.0 : 0.0;
    SPFLOAT fader = 0;
    sp_tenv_compute(sp, ud->master, &mtick, &fader);
    sp->out[0] *= fader;
    sp_progress_compute(sp, ud->prog, NULL, NULL);

}

SPFLOAT randf(SPFLOAT max) {
    SPFLOAT num = (SPFLOAT) rand() / (SPFLOAT) (RAND_MAX / max);
    return num;
}

int main()
{
    srand(time(NULL));
    int i;
    UserData ud;
    ud.pdel = 0;
    sp_data *sp;
    sp_create(&sp);
    sp->len = 44100 * 40;
    char *notes[] = {
        "62 69 78",
        "55 62 69 71",
        "43 50"
    };


    for(i = 0; i < NUMLINE; i++) {
        sp_randi_create(&ud.line[i].randi);
        chord_cloud_create(&ud.line[i].cc);
        chord_cloud_init(sp, ud.line[i].cc, notes[i], 0.1);
        sp_randi_init(sp, ud.line[i].randi);
        ud.line[i].randi->cps = 0.1 + randf(1.5);
    }
    sp_revsc_create(&ud.rev);
    sp_revsc_init(sp, ud.rev);
    ud.rev->feedback = 0.95;
    sp_metro_create(&ud.clk);
    sp_metro_init(sp, ud.clk);
    ud.clk->freq = 86.0 / 60.0;
    sp_count_create(&ud.meter);
    sp_count_init(sp, ud.meter);
    ud.meter->count = 5;
    sp_drip_create(&ud.drip);
    sp_drip_init(sp, ud.drip, 0.01);
    sp_vdelay_create(&ud.del);
    /* give some headroom for the delay */
    sp_vdelay_init(sp, ud.del, 0.4);
    ud.del->del = 0.3;

    sp_tenv_create(&ud.master);
    sp_tenv_init(sp, ud.master);
    ud.master->atk = 0;
    ud.master->hold = 30;
    ud.master->rel =  10;


    sp_progress_create(&ud.prog);
    sp_progress_init(sp, ud.prog);

    modal_create(&ud.mod);
    modal_init(sp, ud.mod);
    sp_ftbl_create(sp, &ud.notes, 1);
    sp_gen_vals(sp, ud.notes, "62 64 59 57");
    sp_tseq_create(&ud.seq);
    sp_tseq_init(sp, ud.seq, ud.notes);
    sp_maygate_create(&ud.mg);
    sp_maygate_init(sp, ud.mg);
    ud.mg->prob = 0.3;
    ud.mg->mode = 1;
    sp_process(sp, &ud, process);

    for(i = 0; i < NUMLINE; i++) {
        sp_randi_destroy(&ud.line[i].randi);
        chord_cloud_destroy(&ud.line[i].cc);
    }


    sp_drip_destroy(&ud.drip);
    sp_revsc_destroy(&ud.rev);
    sp_metro_destroy(&ud.clk);
    sp_count_destroy(&ud.meter);
    sp_vdelay_destroy(&ud.del);
    sp_tenv_destroy(&ud.master);
    modal_destroy(&ud.mod);
    sp_ftbl_destroy(&ud.notes);
    sp_tseq_destroy(&ud.seq);
    sp_maygate_destroy(&ud.mg);
    sp_progress_destroy(&ud.prog);

    sp_destroy(&sp);
    return 0;
}

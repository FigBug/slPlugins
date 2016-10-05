#include <stdlib.h>
#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_mode *mode[4];
    SPFLOAT *mfreq[4];
    SPFLOAT *Q[4];
    SPFLOAT amp;
    SPFLOAT *freq;
    SPFLOAT lfreq;
} modal;

typedef struct {
    modal *mod;
    sp_metro *met;
    sp_ftbl *notes;
    sp_tseq *seq;
} UserData;

static int modal_create(modal **md)
{
    *md = malloc(sizeof(modal));
    return SP_OK;
}

static int modal_init(sp_data *sp, modal *md)
{
    int i;
    md->amp = 0.5;


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

static int modal_compute(sp_data *sp, modal *md, SPFLOAT *in, SPFLOAT *out)
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

static int modal_destroy(modal **md)
{
    int i;
    modal *mdp = *md;
    for(i = 0; i < 4; i++) {
        sp_mode_destroy(&mdp->mode[i]);
    }

    free(*md);
    return SP_OK;
}

int t_mode(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    UserData ud;
    SPFLOAT met = 0, mod = 0, nn;

    modal_create(&ud.mod);
    modal_init(sp, ud.mod);

    sp_metro_create(&ud.met);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 3.0;

    sp_ftbl_create(sp, &ud.notes, 1);
    sp_gen_vals(sp, ud.notes, "60 67 62 69 76");

    sp_tseq_create(&ud.seq);
    sp_tseq_init(sp, ud.seq, ud.notes);

    for(n = 0; n < tst->size; n++) {
        met = 0; 
        mod = 0; 
        nn = 0;
        sp_metro_compute(sp, ud.met, NULL, &met);
        sp_tseq_compute(sp, ud.seq, &met, &nn);
        *ud.mod->freq = sp_midi2cps(nn);
        modal_compute(sp, ud.mod, &met, &mod);
        sp->out[0] = mod;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    modal_destroy(&ud.mod);
    sp_metro_destroy(&ud.met);
    sp_ftbl_destroy(&ud.notes);
    sp_tseq_destroy(&ud.seq);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

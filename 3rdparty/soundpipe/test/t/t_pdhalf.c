#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_metro *met;
    sp_ftbl *ft; 
    sp_tabread *tab;
    sp_phasor *phs;
    sp_tenv *tenv;
    sp_pdhalf *pdhalf;
    sp_scale *scl;
    sp_tseq *ts;
    sp_ftbl *seq; 
} UserData;

int t_pdhalf(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT met = 0;
    SPFLOAT tenv = 0;
    SPFLOAT phs = 0;
    SPFLOAT pd = 0;
    SPFLOAT tr = 0;
    SPFLOAT amt = 0;
    SPFLOAT rev = 0;
    SPFLOAT frq = 0;

    UserData ud;
    sp_srand(sp, 1234567);

    sp_pdhalf_create(&ud.pdhalf);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_gen_sine(sp, ud.ft);
    sp_metro_create(&ud.met);

    sp_tabread_create(&ud.tab);
    sp_tenv_create(&ud.tenv);
    sp_phasor_create(&ud.phs);
    sp_scale_create(&ud.scl);
    sp_ftbl_create(sp, &ud.seq, 1);
    sp_gen_vals(sp, ud.seq, "62 67 69 72");
    sp_tseq_create(&ud.ts);


    sp_pdhalf_init(sp, ud.pdhalf);
    sp_tabread_init(sp, ud.tab, ud.ft, 1);
    sp_phasor_init(sp, ud.phs, 0);
    ud.phs->freq = 440;
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.001;
    ud.tenv->hold = 0.01;
    ud.tenv->rel = 0.2;
    sp_metro_init(sp, ud.met);
    ud.met->freq = 8;
    sp_scale_init(sp, ud.scl);
    ud.scl->min = -0.8;
    ud.scl->max = 0;
    sp_tseq_init(sp, ud.ts, ud.seq);


    for(n = 0; n < tst->size; n++) {
        met = 0;
        tenv = 0;
        phs = 0;
        pd = 0;
        tr = 0;
        amt = 0;
        rev = 0;
        frq = 0;

        sp_metro_compute(sp, ud.met, NULL, &met);
        sp_tenv_compute(sp, ud.tenv, &met, &tenv);
        sp_tseq_compute(sp, ud.ts, &met, &frq);
        frq = sp_midi2cps(frq);
        ud.phs->freq = frq;
        sp_phasor_compute(sp, ud.phs, NULL, &phs);
        rev = 1 - tenv;
        sp_scale_compute(sp, ud.scl, &rev, &amt);
        ud.pdhalf->amount = amt;
        sp_pdhalf_compute(sp, ud.pdhalf, &phs, &pd);
        ud.tab->index = pd;
        sp_tabread_compute(sp, ud.tab, NULL, &tr);

        sp_test_add_sample(tst, tr * tenv);
    }

    fail = sp_test_verify(tst, hash);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

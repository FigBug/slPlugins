#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_tblrec *tblrec;
    sp_ftbl *ft; 
    sp_metro *met;
    sp_pluck *plk;
    sp_randi *randi;
    sp_tabread *tr;
} UserData;

int t_tblrec(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT trig, pluck, rand, tr;
    SPFLOAT tick = (sp->pos == 0 ? 1 : 0);

    sp_srand(sp, 1234567);
    UserData ud;

    sp_tblrec_create(&ud.tblrec);
    sp_ftbl_create(sp, &ud.ft, sp->sr * 0.5);

    sp_metro_create(&ud.met);
    sp_pluck_create(&ud.plk); 
    sp_randi_create(&ud.randi);
    sp_tabread_create(&ud.tr);

    sp_tblrec_init(sp, ud.tblrec, ud.ft);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 2.5;
    sp_pluck_init(sp, ud.plk, 110);
    ud.plk->freq = 440;
    sp_randi_init(sp, ud.randi);
    sp_tabread_init(sp, ud.tr, ud.ft, 1);

    for(n = 0; n < tst->size; n++) {
        trig = 0; 
        pluck = 0;
        rand = 0;
        tr = 0;
        tick = (sp->pos == 0 ? 1 : 0);
        sp_metro_compute(sp, ud.met, NULL, &trig);
        sp_pluck_compute(sp, ud.plk, &trig, &pluck);
        sp_tblrec_compute(sp, ud.tblrec, &pluck, &tick, NULL);
        sp_randi_compute(sp, ud.randi, NULL, &rand);
        ud.tr->index = rand;
        sp_tabread_compute(sp, ud.tr, NULL, &tr);
        sp_test_add_sample(tst, tr);
    }

    fail = sp_test_verify(tst, hash);

    sp_tblrec_destroy(&ud.tblrec);
    sp_ftbl_destroy(&ud.ft);
    sp_metro_destroy(&ud.met);
    sp_pluck_destroy(&ud.plk); 
    sp_randi_destroy(&ud.randi);
    sp_tabread_destroy(&ud.tr);


    if(fail) return SP_NOT_OK;
    /* fail by default */
    else return SP_OK;
}

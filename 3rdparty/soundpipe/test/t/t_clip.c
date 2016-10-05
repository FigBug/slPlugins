#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
   sp_ftbl *ft;
   sp_osc *osc;
   sp_clip *clp;
   sp_moogladder *filt;
} UserData;

int t_clip(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT out, osc, filt;
    
    UserData ud;
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

    for(n = 0; n < tst->size; n++) {
        out = 0 , osc = 0 , filt = 0;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_moogladder_compute(sp, ud.filt, &osc, &filt);
        sp_clip_compute(sp, ud.clp, &filt, &out);
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);

    sp_osc_destroy(&ud.osc);
    sp_clip_destroy(&ud.clp);
    sp_ftbl_destroy(&ud.ft);
    sp_moogladder_destroy(&ud.filt);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

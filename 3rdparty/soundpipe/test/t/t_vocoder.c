#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_vocoder *vocoder;
    sp_blsaw *saw[3];
    sp_diskin *diskin;
} UserData;

int t_vocoder(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT diskin = 0, vocoder = 0, saw = 0, tmp = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    int i;

    int scale[] = {58, 65, 72};

    sp_vocoder_create(&ud.vocoder);
    sp_vocoder_init(sp, ud.vocoder);
   
    sp_diskin_create(&ud.diskin); 
    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");

    for(i = 0; i < 3; i++) {
        sp_blsaw_create(&ud.saw[i]);
        sp_blsaw_init(sp, ud.saw[i]);
        *ud.saw[i]->freq = sp_midi2cps(scale[i]);
    }

    for(n = 0; n < tst->size; n++) {
        diskin = 0; 
        vocoder = 0; 
        saw = 0; 
        tmp = 0;
        sp_diskin_compute(sp, ud.diskin, NULL, &diskin);
        for(i = 0; i < 3; i++) {
            sp_blsaw_compute(sp, ud.saw[i], NULL, &tmp);
            saw += tmp;
        }
        saw *= 0.2;
        sp_vocoder_compute(sp, ud.vocoder, &diskin, &saw, &vocoder);
        sp_test_add_sample(tst, vocoder);
    }

    fail = sp_test_verify(tst, hash);

    sp_vocoder_destroy(&ud.vocoder);

    for(i = 0; i < 3; i++) {
        sp_blsaw_destroy(&ud.saw[i]);
    }

    sp_diskin_destroy(&ud.diskin); 

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

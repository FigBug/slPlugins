#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_phaser *phaser;
    sp_diskin *disk;
} UserData;

int t_phaser(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    SPFLOAT disk1 = 0, disk2 = 0, phaser = 0, foo = 0;
    UserData ud;

    sp_phaser_create(&ud.phaser);
    sp_diskin_create(&ud.disk);

    sp_diskin_init(sp, ud.disk, SAMPDIR "oneart.wav");
    sp_phaser_init(sp, ud.phaser);
    
    for(n = 0; n < tst->size; n++) {
        disk1 = 0; disk2 = 0; phaser = 0; foo = 0;
        sp_diskin_compute(sp, ud.disk, NULL, &disk1);
        disk2 = disk1;
        sp_phaser_compute(sp, ud.phaser, &disk1, &disk2, &phaser, &foo);
        sp_test_add_sample(tst, phaser);
    }

    fail = sp_test_verify(tst, hash);

    sp_phaser_destroy(&ud.phaser);
    sp_diskin_destroy(&ud.disk);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

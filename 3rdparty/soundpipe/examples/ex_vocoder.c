#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_vocoder *vocoder;
    sp_blsaw *saw[3];
    sp_diskin *diskin;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    int i;
    SPFLOAT diskin = 0, vocoder = 0, saw = 0, tmp = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    for(i = 0; i < 3; i++) {
        sp_blsaw_compute(sp, ud->saw[i], NULL, &tmp);
        saw += tmp;
    }
    saw *= 0.2;
    sp_vocoder_compute(sp, ud->vocoder, &diskin, &saw, &vocoder);
    sp->out[0] = vocoder;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    int i;

    int scale[] = {58, 65, 72};

    sp_vocoder_create(&ud.vocoder);
    sp_vocoder_init(sp, ud.vocoder);
   
    sp_diskin_create(&ud.diskin); 
    sp_diskin_init(sp, ud.diskin, "oneart.wav");

    for(i = 0; i < 3; i++) {
        sp_blsaw_create(&ud.saw[i]);
        sp_blsaw_init(sp, ud.saw[i]);
        *ud.saw[i]->freq = sp_midi2cps(scale[i]);
    }

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_vocoder_destroy(&ud.vocoder);
    for(i = 0; i < 3; i++) {
        sp_blsaw_destroy(&ud.saw[i]);
    }
    sp_diskin_destroy(&ud.diskin); 

    sp_destroy(&sp);
    return 0;
}

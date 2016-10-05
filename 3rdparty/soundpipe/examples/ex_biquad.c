#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_noise *ns;
    sp_biquad *tn;
} UserData;

void write_noise(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    sp_noise_compute(sp, ud->ns, NULL, &in);
    sp_biquad_compute(sp, ud->tn, &in, &sp->out[0]); 
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_noise_create(&ud.ns);
    sp_biquad_create(&ud.tn);
    sp_noise_init(sp, ud.ns);
    sp_biquad_init(sp, ud.tn);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_noise);
    sp_noise_destroy(&ud.ns);
    sp_biquad_destroy(&ud.tn);
    sp_destroy(&sp);
    return 0;
}

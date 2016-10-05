#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_noise *ns;
    sp_reson *reson;
} udata;

void write_noise(sp_data *data, void *ud) {
    udata *udata = ud;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    sp_noise_compute(data, udata->ns, NULL, &in);
    udata->reson->freq = 4000;
    udata->reson->bw = 1000;
    sp_reson_compute(data, udata->reson, &in, &out); 
    data->out[0] = out * 0.1;
}
int main() {
    udata ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 12345);
    sp_noise_create(&ud.ns);
    sp_reson_create(&ud.reson);
    sp_noise_init(sp, ud.ns);
    ud.ns->amp = 1.0;
    sp_reson_init(sp, ud.reson);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_noise);
    sp_noise_destroy(&ud.ns);
    sp_reson_destroy(&ud.reson);
    sp_destroy(&sp);
    return 0;
}

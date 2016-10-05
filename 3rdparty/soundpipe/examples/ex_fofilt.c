#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_noise *ns;
    sp_fofilt *tn;
} UserData;

void write_noise(sp_data *data, void *ud) {
    UserData *udata = ud;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    sp_noise_compute(data, udata->ns, NULL, &in);
    sp_fofilt_compute(data, udata->tn, &in, &data->out[0]); 
}
int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_noise_create(&ud.ns);
    sp_fofilt_create(&ud.tn);
    sp_noise_init(sp, ud.ns);
    sp_fofilt_init(sp, ud.tn);
    ud.tn->freq = 500;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_noise);
    sp_noise_destroy(&ud.ns);
    sp_fofilt_destroy(&ud.tn);
    sp_destroy(&sp);
    return 0;
}

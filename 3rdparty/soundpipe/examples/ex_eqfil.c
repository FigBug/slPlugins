#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "soundpipe.h"

typedef struct {
    sp_noise *ns;
    sp_eqfil *eqfil;
    int counter;
} UserData;

void write_noise(sp_data *data, void *ud) {
    UserData *udata = ud;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    if(udata->counter == 0) {
        udata->eqfil->freq = 500 + rand() % 4000;
        udata->eqfil->bw = fabs(udata->eqfil->freq * 0.5);
    }
    sp_noise_compute(data, udata->ns, NULL, &in);
    sp_eqfil_compute(data, udata->eqfil, &in, &data->out[0]); 
    udata->counter = (udata->counter + 1) % 5000;
}
int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);
    sp_noise_create(&ud.ns);
    sp_eqfil_create(&ud.eqfil);
    sp_noise_init(sp, ud.ns);
    sp_eqfil_init(sp, ud.eqfil);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_noise);
    sp_noise_destroy(&ud.ns);
    sp_eqfil_destroy(&ud.eqfil);
    sp_destroy(&sp);
    return 0;
}

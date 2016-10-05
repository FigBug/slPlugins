#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_noise *ns;
    sp_revsc *rev;
    int counter;
} UserData;

void process(sp_data *data, void *ud) {
    UserData *udata = ud;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    SPFLOAT foo = 0;
    sp_noise_compute(data, udata->ns, NULL, &in);
    
    if(udata->counter < 2000) {
        udata->counter = (udata->counter + 1) % 5000;
    }else{
        in = 0;
    }
    sp_revsc_compute(data, udata->rev, &in, &in, &data->out[0], &foo); 
}

int main() {
    srand(time(NULL));
    UserData ud;
    ud.counter = 0;
    sp_data *sp;
    sp_create(&sp);
    sp_noise_create(&ud.ns);
    sp_revsc_create(&ud.rev);
    sp_noise_init(sp, ud.ns);
    sp_revsc_init(sp, ud.rev);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_noise_destroy(&ud.ns);
    sp_revsc_destroy(&ud.rev);
    sp_destroy(&sp);
    return 0;
}

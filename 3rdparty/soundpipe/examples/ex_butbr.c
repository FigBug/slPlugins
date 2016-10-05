#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_butbr *butbr;
    sp_noise *ns;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT noise = 0, butbr = 0;
    sp_noise_compute(sp, ud->ns, NULL, &noise);
    sp_butbr_compute(sp, ud->butbr, &noise, &butbr);
    sp->out[0] = butbr;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_butbr_create(&ud.butbr);
    sp_noise_create(&ud.ns);

    sp_butbr_init(sp, ud.butbr);
    sp_noise_init(sp, ud.ns);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_butbr_destroy(&ud.butbr);
    sp_noise_destroy(&ud.ns);

    sp_destroy(&sp);
    return 0;
}

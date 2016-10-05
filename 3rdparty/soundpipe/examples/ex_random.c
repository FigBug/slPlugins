#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_random *random;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT random = 0;
    sp_random_compute(sp, ud->random, NULL, &random);
    sp->out[0] = random;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_random_create(&ud.random);

    sp_random_init(sp, ud.random);
    ud.random->min = -0.2;
    ud.random->max = 0.2;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_random_destroy(&ud.random);

    sp_destroy(&sp);
    return 0;
}

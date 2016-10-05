#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_gbuzz *buzz;
    sp_ftbl *ft; 
    int counter;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    sp_gbuzz_compute(sp, ud->buzz, NULL, &sp->out[0]);
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_gbuzz_create(&ud.buzz);

    sp_gen_sine(sp, ud.ft);
    sp_gbuzz_init(sp, ud.buzz, ud.ft, 0);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_ftbl_destroy(&ud.ft);
    sp_gbuzz_destroy(&ud.buzz);
    sp_destroy(&sp);
    return 0;
}

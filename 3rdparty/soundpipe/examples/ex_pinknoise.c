#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_pinknoise *ns;
} UserData;

void write_pinknoise(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT out = 0;
    sp_pinknoise_compute(sp, ud->ns, NULL, &out);
    sp_out(sp, 0, out);
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_pinknoise_create(&ud.ns);
    sp_pinknoise_init(sp, ud.ns);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_pinknoise);
    sp_pinknoise_destroy(&ud.ns);
    sp_destroy(&sp);
    return 0;
}

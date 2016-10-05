/*
 * This module reads from standard input, and writes it to a wave file.
 * 
 * To write 5 seconds of white noise:
 *
 * cat /dev/urandom | ./ex_in.bin
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_in *in;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT in = 0;
    sp_in_compute(sp, ud->in, NULL, &in);
    sp->out[0] = in;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_in_create(&ud.in);

    sp_in_init(sp, ud.in);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_in_destroy(&ud.in);

    sp_destroy(&sp);
    return 0;
}

/*
 * This is a dummy example.
 * Please implement a small and simple working example of your module, and then
 * remove this header.
 * Don't be clever.
 * Bonus points for musicality. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_slice *slice;
    sp_ftbl *vals; 
    sp_ftbl *buf; 
    sp_dmetro *met;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT slice = 0;
    SPFLOAT dm = 0;
    sp_dmetro_compute(sp, ud->met, NULL, &dm);
    ud->slice->id = 2;
    sp_slice_compute(sp, ud->slice, &dm, &slice);
    sp_out(sp, 0, slice);
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_slice_create(&ud.slice);
    sp_ftbl_create(sp, &ud.vals, 1);
    sp_ftbl_loadfile(sp, &ud.buf, "oneart.wav");
    sp_dmetro_create(&ud.met);

    sp_gen_vals(sp, ud.vals, 
    "6770 96139 159104 228847");

    sp_slice_init(sp, ud.slice, ud.vals, ud.buf);
    sp_dmetro_init(sp, ud.met);
    ud.met->time = 0.75;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_slice_destroy(&ud.slice);
    sp_dmetro_destroy(&ud.met);
    sp_ftbl_destroy(&ud.buf);
    sp_ftbl_destroy(&ud.vals);

    sp_destroy(&sp);
    return 0;
}

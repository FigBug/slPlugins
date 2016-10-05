#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "soundpipe.h"

#define NVOICES 5


typedef struct oscil{
    sp_osc *osc;
    sp_randi *rnd;
}oscil;

typedef struct {
    oscil v[NVOICES];
    sp_ftbl *ft; 
    sp_revsc *rev;
    sp_noise *ns;
    sp_dcblock *dcblk;
} udata;

SPFLOAT midi2cps(int nn){
    return pow(2, (nn - 69.0) / 12.0) * 440.0;
}

void write_osc(sp_data *data, void *ud) {
    udata *udp = ud;
    int i;
    SPFLOAT amp = 0;
    SPFLOAT osc = 0;
    SPFLOAT dry = 0;
    SPFLOAT wet = 0;
    SPFLOAT blk = 0;
    SPFLOAT ns = 0;
    SPFLOAT dummy = 0;

    for(i = 0; i < NVOICES; i++){
        sp_randi_compute(data, udp->v[i].rnd, NULL, &amp);
        sp_osc_compute(data, udp->v[i].osc, NULL, &osc);
        dry += osc * amp; 
    }
    sp_noise_compute(data, udp->ns, NULL, &ns);
    sp_revsc_compute(data, udp->rev, &dry, &dry, &wet, &dummy);  

    sp_dcblock_compute(data, udp->dcblk, &wet, &blk);
    data->out[0] = 0.7 * dry + 0.2 * wet; //+ ns * 0.002;
    data->out[0] += 1; 
    data->out[0] *= 0.5; 
    //data->out = 0.0;
}

int main() {
    srand(time(NULL));
    udata ud;
    sp_data *sp;
    sp_create(&sp);
    /* this is the samplerate I typically use for realtime */
    sp->sr = 22050;
    int notes[] = { 60, 67, 71, 74, 76 };
    int i;
    sp_noise_create(&ud.ns);
    sp_dcblock_create(&ud.dcblk);
    sp_revsc_create(&ud.rev);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_noise_init(sp, ud.ns);
    sp_gen_line(sp, ud.ft, "0 1 2048 -1");
    for(i = 0; i < NVOICES; i++){ 
        sp_osc_create(&ud.v[i].osc);
        sp_randi_create(&ud.v[i].rnd);
        sp_osc_init(sp, ud.v[i].osc, ud.ft);
        ud.v[i].osc->amp = 0.1;
        ud.v[i].osc->freq = midi2cps(notes[i]);
        sp_randi_init(sp, ud.v[i].rnd, rand());
        ud.v[i].rnd->cps = 0.8;
    }

    sp_revsc_init(sp, ud.rev);
    sp_dcblock_init(sp, ud.dcblk);
    /* length does not work in realtime */
    sp->len = 0;

    sp_rpi_process(sp, &ud, write_osc);

    sp_revsc_destroy(&ud.rev);
    sp_dcblock_destroy(&ud.dcblk);
    sp_noise_destroy(&ud.ns);
    sp_ftbl_destroy(&ud.ft);
    for(i = 0; i < NVOICES; i++){
        sp_osc_destroy(&ud.v[i].osc);
        sp_randi_destroy(&ud.v[i].rnd);
    }
    sp_destroy(&sp);
    return 0;
}

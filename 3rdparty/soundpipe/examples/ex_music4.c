#include <stdio.h>
#include "soundpipe.h"

#define NOTEDUR 8000

typedef struct{
    sp_posc3 *posc[8];
    sp_tenv *tenv[8];
    sp_revsc *rev;
    sp_ftbl *ft;
}UserData;

void make_note(sp_data *sp, sp_tenv *tenv, sp_posc3 *posc3, unsigned long counter, unsigned long clock, SPFLOAT *output)
{
    unsigned long count = counter % clock;
    SPFLOAT env = 0, osc = 0;
    SPFLOAT tick = (count == 0) ? 1 : 0;
    sp_tenv_compute(sp, tenv, &tick, &env);
    sp_posc3_compute(sp, posc3, NULL, &osc);
    *output = osc * env;
}
void write_music(sp_data *sp, void *ud)
{
    UserData *udp = ud;
    SPFLOAT out[8];
    SPFLOAT mix, verb, foo;
    for(int i = 0; i < 8; i++) {
        make_note(sp, udp->tenv[i], udp->posc[i], sp->pos, ((i * NOTEDUR) + NOTEDUR), &out[i]);
    }
    mix = (out[0] * 0.2) + (out[1] * 0.2) + (out[2] * 0.2) + (out[3] * 0.2) + (out[4] * 0.2) + (out[5] * 0.2)
       + (out[6] * 0.2) +  (out[7] * 0.2);
    sp_revsc_compute(sp, udp->rev, &mix, &mix, &verb, &foo);
    sp->out[0] = mix + (verb * 0.2);
}

int main()
{
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 4096);
    for(int i = 0; i < 8; i++) sp_posc3_create(&ud.posc[i]);
    for(int i = 0; i < 8; i++) sp_tenv_create(&ud.tenv[i]);
    sp_revsc_create(&ud.rev);
    sp_gen_sine(sp, ud.ft);

    for(int i = 0; i < 8; i++) {
        sp_tenv_init(sp, ud.tenv[i]);
        ud.tenv[i]->atk = 0.01;
        ud.tenv[i]->hold = 0.1;
        ud.tenv[i]->rel =  0.1;
    }
    for(int i = 0; i < 8; i++) sp_posc3_init(sp, ud.posc[i], ud.ft);
    ud.posc[0]->freq = sp_midi2cps(48);
    ud.posc[1]->freq = sp_midi2cps(52);
    ud.posc[2]->freq = sp_midi2cps(59);
    ud.posc[3]->freq = sp_midi2cps(62);
    ud.posc[4]->freq = sp_midi2cps(72);
    ud.posc[5]->freq = sp_midi2cps(71);
    ud.posc[6]->freq = sp_midi2cps(69);
    ud.posc[7]->freq = sp_midi2cps(66);

    sp_revsc_init(sp, ud.rev);

    sp->len = 44100 * 20;
    sp_process(sp, &ud, write_music);

    for(int i = 0; i < 8; i++) sp_posc3_destroy(&ud.posc[i]);
    for(int i = 0; i < 8; i++) sp_tenv_destroy(&ud.tenv[i]);
    sp_revsc_destroy(&ud.rev);
    sp_ftbl_destroy(&ud.ft);
    sp_destroy(&sp);
    return 0;
}

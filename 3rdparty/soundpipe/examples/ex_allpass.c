#include "soundpipe.h"

typedef struct {
    sp_allpass *ap;
    sp_tenv *env;
    sp_noise *nz;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT tick = 0, env = 0, noise = 0, allpass = 0;

    tick = (sp->pos == 0) ? 1 : 0;
    sp_tenv_compute(sp, ud->env, &tick, &env);
    sp_noise_compute(sp, ud->nz, NULL, &noise);
    noise *= env * 0.5;
    sp_allpass_compute(sp, ud->ap, &noise, &allpass);

    sp->out[0] = allpass;
}


int main()
{
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_allpass_create(&ud.ap);
    sp_tenv_create(&ud.env);
    sp_noise_create(&ud.nz);

    sp_allpass_init(sp, ud.ap, 0.1);
    sp_tenv_init(sp, ud.env);
    ud.env->atk = 0.001;
    ud.env->hold = 0.00;
    ud.env->rel =  0.1;

    sp_noise_init(sp, ud.nz);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_noise_destroy(&ud.nz);
    sp_tenv_destroy(&ud.env);
    sp_allpass_destroy(&ud.ap);

    sp_destroy(&sp);
    return 0;
}

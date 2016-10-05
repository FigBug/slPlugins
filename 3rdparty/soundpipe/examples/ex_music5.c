#include "soundpipe.h"

typedef struct {
    uint16_t counter;
    uint16_t t;
    SPFLOAT out;
} UserData;

void process(sp_data *sp, void *udata) 
{
    UserData *ud = udata;
    if(ud->counter == 0) {
        uint16_t t = ud->t;

        ud->out = 
            (SPFLOAT)((t*(((t>>12)|(t>>8))&(63&(t>>4)))) & 0xFF);
        ud->out /= 0xFF;
        ud->out -= 0.5;
        ud->out *= 0.3;
        ud->t++;
    }
    sp->out[0] = ud->out;
    ud->counter++;
    ud->counter %= 10;
}

int main()
{
    UserData ud;
    sp_data *sp;
    ud.counter = 0;
    ud.t = 0;
    sp_create(&sp);
    sp->len = 44100 * 15;
    sp_process(sp, &ud, process);
    sp_destroy(&sp);
    return 0;
}

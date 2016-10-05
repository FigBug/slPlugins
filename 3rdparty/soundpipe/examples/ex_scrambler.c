#include "soundpipe.h"

typedef struct {
    sp_ftbl *src;
    sp_ftbl *dest;
} UserData;

static void process(sp_data *sp, void *userdata)
{
    UserData *ud = userdata;
    sp_out(sp, 0, ud->dest->tbl[sp->pos]);
}

int main()
{
    sp_data *sp;
    UserData ud;
    sp_create(&sp);
    sp_srand(sp, 12345);
    sp_ftbl_loadfile(sp, &ud.src, "oneart.wav");
    sp_gen_scrambler(sp, ud.src, &ud.dest);
    sp->len = ud.src->size;
    sp_process(sp, &ud, process);
    sp_ftbl_destroy(&ud.src);
    sp_ftbl_destroy(&ud.dest);
    sp_destroy(&sp);
    return 0;
}

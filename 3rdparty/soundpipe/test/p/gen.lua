name = arg[1]
sptbl = {}

dofile ("../../sp_dict.lua")

header=[[
#include <stdlib.h>
#include <stdio.h>
#include "soundpipe.h"
#include "config.h"

int main() {
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 12345);
    sp->sr = SR;
    sp->len = sp->sr * LEN;
    uint32_t t, u;
    SPFLOAT in = 0, out = 0;
]]

compute=[[
    sp_FOO *unit[NUM];

    for(u = 0; u < NUM; u++) { 
        sp_FOO_create(&unit[u]);
        sp_FOO_init(sp, unit[u]);
    }

    for(t = 0; t < sp->len; t++) {
        for(u = 0; u < NUM; u++) sp_FOO_compute(sp, unit[u], &in, &out);
    }

    for(u = 0; u < NUM; u++) sp_FOO_destroy(&unit[u]);
]]


footer=[[
    sp_destroy(&sp);
    return 0;
}
]]

compute = string.gsub(compute, "FOO", name)

print(header)
print(compute)
print(footer)

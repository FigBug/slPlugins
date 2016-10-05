#include <stdlib.h>
#include <string.h>
#include "soundpipe.h"

int sp_diskin_create(sp_diskin **p)
{
    *p = malloc(sizeof(sp_diskin));
    return SP_OK;
}

int sp_diskin_destroy(sp_diskin **p)
{
    sp_diskin *pp = *p;
    if(pp->loaded) sf_close(pp->file);
    free(*p);
    return SP_OK;
}

int sp_diskin_init(sp_data *sp, sp_diskin *p, const char *filename)
{
    p->info.format = 0;
    memset(&p->info, 0, sizeof(SF_INFO));
    p->file = sf_open(filename, SFM_READ, &p->info);
    p->loaded = 0;
    p->bufpos = 0;

    if(p->file == NULL) {
        fprintf(stderr, "Error: could not open file \"%s\"\n", filename);
        exit(1);
    }

    if(p->info.channels != 1) {
        fprintf(stderr, "Warning: file \"%s\" has %d channels," 
                "when it is expecting only 1\n", filename, p->info.channels);
    }

    p->loaded = 1;

    if(p->info.frames < 1024) {
        p->count = p->info.frames;
    } else {
        p->count = 1024;
    }
    memset(p->buffer, 0, sizeof(SPFLOAT) * 1024);
    return SP_OK;
}

int sp_diskin_compute(sp_data *sp, sp_diskin *p, SPFLOAT *in, SPFLOAT *out)
{
    if(p->bufpos == 0 && p->loaded && p->count > 0) {
        p->count = sf_read_float(p->file, p->buffer, p->count);
    } 

    if(p->count <= 0) {
        *out = 0;
        return SP_OK;
    }

    *out = p->buffer[p->bufpos++];
    p->bufpos %= 1024; 
    return SP_OK;
}

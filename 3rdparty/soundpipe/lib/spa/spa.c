#include <stdio.h>
#include <stdint.h>
#include "soundpipe.h"

int spa_open(sp_data *sp, sp_audio *spa, const char *name, int mode)
{
    spa->mode = SPA_NULL;
    spa_header *header = &spa->header;
    spa->offset = sizeof(spa_header);
    if(mode == SPA_READ) {
        spa->fp = fopen(name, "rb");
        fread(header, spa->offset, 1, spa->fp);
    } else if(mode == SPA_WRITE) {
        spa->fp = fopen(name, "wb");
        header->magic = 100;
        header->nchan = sp->nchan;
        header->len = sp->len;
        header->sr = sp->sr;
        fwrite(header, spa->offset, 1, spa->fp);
    } else {
        return SP_NOT_OK;
    }

    spa->mode = mode;

    /* TODO: file error checking */

    return SP_OK;
}

int spa_write_buf(sp_data *sp, sp_audio *spa, SPFLOAT *buf, uint32_t size)
{
    if(spa->mode != SPA_WRITE) {
        return SP_NOT_OK;
    }
    fwrite(buf, sizeof(SPFLOAT), size, spa->fp);
    return SP_OK;
}

int spa_read_buf(sp_data *sp, sp_audio *spa, SPFLOAT *buf, uint32_t size)
{
    if(spa->mode != SPA_READ) {
        return SP_NOT_OK;
    }
    fread(buf, sizeof(SPFLOAT), size, spa->fp);
    return SP_OK;
}

int spa_close(sp_audio *spa)
{
    fclose(spa->fp);
}

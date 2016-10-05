enum { SPA_READ, SPA_WRITE, SPA_NULL };

typedef struct {
    char magic;
    char nchan;
    uint16_t sr;
    uint32_t len;
} spa_header;

typedef struct {
    spa_header header;
    size_t offset;
    int mode;
    FILE *fp;
    uint32_t pos;
} sp_audio;
int spa_open(sp_data *sp, sp_audio *spa, const char *name, int mode);
int spa_write_buf(sp_data *sp, sp_audio *spa, SPFLOAT *buf, uint32_t size);
int spa_read_buf(sp_data *sp, sp_audio *spa, SPFLOAT *buf, uint32_t size);
int spa_close(sp_audio *spa);

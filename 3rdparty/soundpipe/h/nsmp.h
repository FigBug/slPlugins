typedef struct nano_entry {
    char name[50];
    uint32_t pos;
    uint32_t size;
    SPFLOAT speed;
    struct nano_entry *next;
} nano_entry;

typedef struct {
    int nval;
    int init;
    nano_entry root;
    nano_entry *last;
} nano_dict;

typedef struct {
    char ini[100];
    SPFLOAT curpos;
    nano_dict dict;
    int selected;
    nano_entry *sample;
    nano_entry **index; 
    sp_ftbl *ft;
    int sr;
} nanosamp;

typedef struct {
    nanosamp *smp;
    uint32_t index;
    int triggered;
} sp_nsmp;

int sp_nsmp_create(sp_nsmp **p);
int sp_nsmp_destroy(sp_nsmp **p);
int sp_nsmp_init(sp_data *sp, sp_nsmp *p, sp_ftbl *ft, int sr, const char *ini);
int sp_nsmp_compute(sp_data *sp, sp_nsmp *p, SPFLOAT *in, SPFLOAT *out);

int sp_nsmp_print_index(sp_data *sp, sp_nsmp *p);

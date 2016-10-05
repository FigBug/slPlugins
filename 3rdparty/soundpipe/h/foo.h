typedef struct {
    SPFLOAT bar;
} sp_foo;

int sp_foo_create(sp_foo **p);
int sp_foo_destroy(sp_foo **p);
int sp_foo_init(sp_data *sp, sp_foo *p);
int sp_foo_compute(sp_data *sp, sp_foo *p, SPFLOAT *in, SPFLOAT *out);

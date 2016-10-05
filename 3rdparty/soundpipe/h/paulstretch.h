typedef struct {
    uint32_t windowsize;
    uint32_t half_windowsize;
    SPFLOAT stretch;
    SPFLOAT start_pos;
    SPFLOAT displace_pos;
    SPFLOAT *window;
    SPFLOAT *old_windowed_buf;
    SPFLOAT *hinv_buf;
    SPFLOAT *buf;
    SPFLOAT *output;
    sp_ftbl *ft;
    kiss_fftr_cfg fft, ifft;
    kiss_fft_cpx *tmp1, *tmp2;
    uint32_t counter;
    sp_auxdata m_window;
    sp_auxdata m_old_windowed_buf;
    sp_auxdata m_hinv_buf;
    sp_auxdata m_buf;
    sp_auxdata m_output;
} sp_paulstretch;

int sp_paulstretch_create(sp_paulstretch **p);
int sp_paulstretch_destroy(sp_paulstretch **p);
int sp_paulstretch_init(sp_data *sp, sp_paulstretch *p, sp_ftbl *ft, SPFLOAT windowsize, SPFLOAT stretch);
int sp_paulstretch_compute(sp_data *sp, sp_paulstretch *p, SPFLOAT *in, SPFLOAT *out);

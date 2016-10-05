#ifdef USE_FFTW3
#include <fftw3.h>
#endif

#define fftw_real double
#define rfftw_plan fftw_plan

typedef struct FFTFREQS {
    int size;
    SPFLOAT *s,*c;
} FFTFREQS;

typedef struct {
    int fftsize;
#ifdef USE_FFTW3
    fftw_real *tmpfftdata1, *tmpfftdata2;
    rfftw_plan planfftw,planfftw_inv;
#else
    kiss_fftr_cfg fft, ifft;
    kiss_fft_cpx *tmp1, *tmp2;
#endif
} FFTwrapper;

void FFTwrapper_create(FFTwrapper **fw, int fftsize);
void FFTwrapper_destroy(FFTwrapper **fw);

void newFFTFREQS(FFTFREQS *f, int size);
void deleteFFTFREQS(FFTFREQS *f);

void smps2freqs(FFTwrapper *ft, SPFLOAT *smps, FFTFREQS *freqs);
void freqs2smps(FFTwrapper *ft, FFTFREQS *freqs, SPFLOAT *smps);

typedef struct {
    void *faust;
    int argpos;
    SPFLOAT *args[10];
    SPFLOAT *MaxNotch1Freq;
    SPFLOAT *MinNotch1Freq;
    SPFLOAT *Notch_width;
    SPFLOAT *NotchFreq;
    SPFLOAT *VibratoMode;
    SPFLOAT *depth;
    SPFLOAT *feedback_gain;
    SPFLOAT *invert;
    SPFLOAT *level;
    SPFLOAT *lfobpm;
} sp_phaser;

int sp_phaser_create(sp_phaser **p);
int sp_phaser_destroy(sp_phaser **p);
int sp_phaser_init(sp_data *sp, sp_phaser *p);
int sp_phaser_compute(sp_data *sp, sp_phaser *p, 
	SPFLOAT *in1, SPFLOAT *in2, SPFLOAT *out1, SPFLOAT *out2);

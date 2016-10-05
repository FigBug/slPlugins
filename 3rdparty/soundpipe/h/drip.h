typedef struct sp_drip{

    SPFLOAT amp; /* How loud */
    SPFLOAT dettack; /* How loud */
    SPFLOAT num_tubes;
    SPFLOAT damp;
    SPFLOAT shake_max;
    SPFLOAT freq;
    SPFLOAT freq1;
    SPFLOAT freq2;

    SPFLOAT num_objectsSave;
    SPFLOAT shake_maxSave;
    SPFLOAT shakeEnergy;
    SPFLOAT outputs00;
    SPFLOAT outputs01;
    SPFLOAT outputs10;
    SPFLOAT outputs11;
    SPFLOAT outputs20;
    SPFLOAT outputs21;
    SPFLOAT coeffs00;
    SPFLOAT coeffs01;
    SPFLOAT coeffs10;
    SPFLOAT coeffs11;
    SPFLOAT coeffs20;
    SPFLOAT coeffs21;
    SPFLOAT finalZ0;
    SPFLOAT finalZ1;
    SPFLOAT finalZ2;
    SPFLOAT sndLevel;
    SPFLOAT gains0;
    SPFLOAT gains1;
    SPFLOAT gains2;
    SPFLOAT center_freqs0;
    SPFLOAT center_freqs1;
    SPFLOAT center_freqs2;
    SPFLOAT soundDecay;
    SPFLOAT systemDecay;
    SPFLOAT num_objects;
    SPFLOAT totalEnergy;
    SPFLOAT decayScale;
    SPFLOAT res_freq0;
    SPFLOAT res_freq1;
    SPFLOAT res_freq2;
    SPFLOAT shake_damp;
    int kloop;
} sp_drip;

int sp_drip_create(sp_drip **p);
int sp_drip_destroy(sp_drip **p);
int sp_drip_init(sp_data *sp, sp_drip *p, SPFLOAT dettack);
int sp_drip_compute(sp_data *sp, sp_drip *p, SPFLOAT *trig, SPFLOAT *out);

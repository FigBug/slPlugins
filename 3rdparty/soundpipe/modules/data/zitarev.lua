sptbl["zitarev"] = {

    files = {
        module = "zitarev.c",
        header = "zitarev.h",
        example = "ex_zitarev.c",
    },

    func = {
        create = "sp_zitarev_create",
        destroy = "sp_zitarev_destroy",
        init = "sp_zitarev_init",
        compute = "sp_zitarev_compute",
    },

    params = {
        optional = {
            {
                name = "in_delay",
                type = "SPFLOAT",
                description = "Delay in ms before reverberation begins.",
                default = 60,
                min = 10,
                max = 100 
                
            },
            {
                name = "lf_x",
                type = "SPFLOAT",
                description ="Crossover frequency separating low and middle frequencies (Hz).",
                default = 200,
                min = 200,
                max = 1000
            },
            {
                name = "rt60_low",
                type = "SPFLOAT",
                description ="Time (in seconds) to decay 60db in low-frequency band.",
                default = 3.0,
                min = 1,
                max = 8 
            },
            {
                name = "rt60_mid",
                type = "SPFLOAT",
                description ="Time (in seconds) to decay 60db in mid-frequency band.",
                default = 2.0,
                min = 1,
                max = 8
            },
            {
                name = "hf_damping",
                type = "SPFLOAT",
                description ="Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60.",
                default = 6000.0,
                min = 1500,
                max = 0.5 * 48000
            },
            {
                name = "eq1_freq",
                type = "SPFLOAT",
                description ="Center frequency of second-order Regalia Mitra peaking equalizer section 1.",
                default = 315.0,
                min = 40,
                max = 2500
            },
            {
                name = "eq1_level",
                type = "SPFLOAT",
                description ="Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1",
                default = 0,
                min = -15,
                max = 15
            },
            {
                name = "eq2_freq",
                type = "SPFLOAT",
                description ="Center frequency of second-order Regalia Mitra peaking equalizer section 2.",
                default = 1500.0,
                min = 160,
                max = 10000
            },
            {
                name = "eq2_level",
                type = "SPFLOAT",
                description ="Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2",
                default = 0 ,
                min = -15,
                max = 15
            },
            {
                name = "mix",
                type = "SPFLOAT",
                description ="0 = all dry, 1 = all wet",
                default = 1,
                min = 0,
                max = 1
            },
            {
                name = "level",
                type = "SPFLOAT",
                description ="Output scale factor (in dB).",
                default = -20,
                min = -70,
                max = 40
            }
        }
    },

    modtype = "module",

    description = [[8 FDN stereo zitareverb algorithm, imported from Faust.]],

    ninputs = 2,
    noutputs = 2,

    inputs = {
        {
            name = "input_1",
            description = "First input."
        },
        {
            name = "input_2",
            description = "Second input."
        },
    },

    outputs = {
        {
            name = "out_1",
            description = "Channel 1 output. Most likely for the left channel."
        },
        {
            name = "out_2",
            description = "Channel 2 output. Mose likely for the right channel."
        },
    }

}

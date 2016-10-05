sptbl["oscmorph"] = {

    files = {
        module = "oscmorph.c",
        header = "oscmorph.h",
        example = "ex_oscmorph.c",
    },

    func = {
        create = "sp_oscmorph_create",
        destroy = "sp_oscmorph_destroy",
        init = "sp_oscmorph_init",
        compute = "sp_oscmorph_compute",
    },

    params = {
        mandatory = {
            {
                name = "tbl",
                type = "sp_ftbl **",
                description = "An array of ftables to read from. Note: the size of these tables must be a power of 2 (and the same size as well).",
                default = "N/A"
            },
            {
                name = "nft",
                type = "int",
                description ="Number of ftbls",
                default = 2
            },
            {
                name = "phase",
                type = "SPFLOAT",
                description ="Initial phase of waveform, expects a value 0-1",
                default = 0
            }
        },
        optional = {
            {
                name = "freq",
                type = "SPFLOAT",
                description = "Frequency (in Hz)",
                default = 440
            },
            {
                name = "amp",
                type = "SPFLOAT",
                description ="Amplitude (typically a value between 0 and 1).",
                default = 0.2
            },
            {
                name = "wtpos",
                type = "SPFLOAT",
                description ="Wavetable position. (scaled 0-1)",
                default = 0.0
            },
        }
    },

    modtype = "module",

    description = [[Wavetable morphing oscillator

This is an oscillator with linear interpolation that is capable of morphing 
between an arbitrary number of wavetables. 
]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

sptbl["fof"] = {

    files = {
        module = "fof.c",
        header = "fof.h",
        example = "ex_fof.c",
    },

    func = {
        create = "sp_fof_create",
        destroy = "sp_fof_destroy",
        init = "sp_fof_init",
        compute = "sp_fof_compute",
    },

    params = {
        mandatory = {
            {
                name = "sine",
                type = "sp_ftbl *",
                description = "ftable for sine wave.",
                default = "N/A"
            },
            {
                name = "win",
                type = "sp_ftbl *",
                description = "Ftable for envelope function (use either gen_line or gen_sinecomp)",
                default = "N/A"
            },
            {
                name = "iolaps",
                type = "int",
                description = "Maximum number of foflet overlaps.",
                default = 100 
            },
            {
                name = "iphs",
                type = "SPFLOAT",
                description = "Phase",
                default = 0
            }
        },

        optional = {
            {
                name = "amp",
                type = "SPFLOAT",
                description = "Overall amplitude",
                default = 0.5
            },
            {
                name = "fund",
                type = "SPFLOAT",
                description = "Fundamental frequency",
                default = 100
            },
            {
                name = "form",
                type = "SPFLOAT",
                description = "Formant frequency.",
                default = 500
            },
            {
                name = "oct",
                type = "SPFLOAT",
                description = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.",
                default = 0
            },
            {
                name = "band",
                type = "SPFLOAT",
                description = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.",
                default = 50
            },
            {
                name = "ris",
                type = "SPFLOAT",
                description = "Rise of sinusoid burst (in seconds)",
                default = 0.003
            },
            {
                name = "dec",
                type = "SPFLOAT",
                description = "Decay of the sinusoid burst (in seconds).",
                default = 0.0007
            },
            {
                name = "dur",
                type = "SPFLOAT",
                description = "OVerall duration of sinusoid burst (in seconds).",
                default = 0.02
            },
        }
    },

    modtype = "module",

    description = [[Produces sinusoid bursts for granular and formant synthesis

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

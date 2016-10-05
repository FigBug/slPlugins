sptbl["fog"] = {

    files = {
        module = "fog.c",
        header = "fog.h",
        example = "ex_fog.c",
    },

    func = {
        create = "sp_fog_create",
        destroy = "sp_fog_destroy",
        init = "sp_fog_init",
        compute = "sp_fog_compute",
    },

    params = {
        mandatory = {
            {
                name = "wav",
                type = "sp_ftbl *",
                description = "ftable for sample.",
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
                description = "Maximum number of foglet overlaps.",
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
                name = "dens",
                type = "SPFLOAT",
                description = "Density. The frequency of grains/second.",
                default = 80 
            },
            {
                name = "trans",
                type = "SPFLOAT",
                description = "Transposition, in terms of playback speed",
                default = 1
            },
            {
                name = "spd",
                type = "SPFLOAT",
                description = "Starting sample index, normalized 0-1.",
                default = 0
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

    description = [[Sucession of grains from data in a stored function table]],

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

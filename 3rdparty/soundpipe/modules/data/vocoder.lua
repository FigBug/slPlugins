sptbl["vocoder"] = {

    files = {
        module = "vocoder.c",
        header = "vocoder.h",
        example = "ex_vocoder.c",
    },

    func = {
        create = "sp_vocoder_create",
        destroy = "sp_vocoder_destroy",
        init = "sp_vocoder_init",
        compute = "sp_vocoder_compute",
    },

    params = {
        mandatory = {
        },

        optional = {
            {
                name = "atk",
                type = "SPFLOAT",
                description = "Attack time . (Range 0.001 and 0.5 seconds)",
                default = 0.1
            },
            {
                name = "rel",
                type = "SPFLOAT",
                description = "Release time",
                default = 0.1
            },
            {
                name = "bwratio",
                type = "SPFLOAT",
                description = "Coeffecient to adjust the bandwidth of each band (0.1 - 2)",
                default = 0.5
            },
            
        }
    },

    modtype = "module",

    description = [[16-band channel vocoder]],

    ninputs = 2,
    noutputs = 1,

    inputs = {
        {
            name = "source",
            description = "Source signal (also known as carrier)."
        },
        {
            name = "excite",
            description = "Excitation signal (also known as modulator)"
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

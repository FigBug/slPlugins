sptbl["bitcrush"] = {

    files = {
        module = "bitcrush.c",
        header = "bitcrush.h",
        example = "ex_bitcrush.c",
    },

    func = {
        create = "sp_bitcrush_create",
        destroy = "sp_bitcrush_destroy",
        init = "sp_bitcrush_init",
        compute = "sp_bitcrush_compute",
    },

    params = {
        optional = {
            {
                name = "bitdepth",
                type = "SPFLOAT",
                description = "Bit depth. Expects an integer in the range of 1-16. Fractional values will be truncated.",
                default = 8
            },
            {
                name = "srate",
                type = "SPFLOAT",
                description ="Sampling rate.",
                default = 10000
            },
        }
    },

    modtype = "module",

    description = [[Digital signal degradation

    Bitcrusher will digitally degrade a signal by altering the bitdepth and sampling-rate. ]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "input",
            description = "Signal input."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output"
        },
    }

}

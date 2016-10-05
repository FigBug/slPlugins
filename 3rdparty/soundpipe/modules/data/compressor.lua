sptbl["compressor"] = {

    files = {
        module = "compressor.c",
        header = "compressor.h",
        example = "ex_compressor.c",
    },

    func = {
        create = "sp_compressor_create",
        destroy = "sp_compressor_destroy",
        init = "sp_compressor_init",
        compute = "sp_compressor_compute",
    },

    params = {
        mandatory = {
        },

        optional = {
            {
                name = "ratio",
                type = "SPFLOAT",
                description = "Ratio to compress with, a value > 1 will compress",
                default = 1
            },
            {
                name = "thresh",
                type = "SPFLOAT",
                description ="Threshold (in dB) 0 = max",
                default = 0
            },
            {
                name = "atk",
                type = "SPFLOAT",
                description ="Compressor attack",
                default = 0.1
            },
            {
                name = "rel",
                type = "SPFLOAT",
                description ="Compressor release",
                default = 0.1
            },
        }
    },

    modtype = "module",

    description = [[Compressor]],

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
            description = "Signal output."
        },
    }

}

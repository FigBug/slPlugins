sptbl["biquad"] = {

    files = {
        module = "biquad.c",
        header = "biquad.h",
        --example = "ex_tone.c",
    },

    func = {
        create = "sp_biquad_create",
        destroy = "sp_biquad_destroy",
        init = "sp_biquad_init",
        compute = "sp_biquad_compute",
    },

    params = {
        optional = {
            {
                name = "b0",
                type = "SPFLOAT",
                description = "biquad coefficient.",
                default = 0
            },
            {
                name = "b1",
                type = "SPFLOAT",
                description = "biquad coefficient.",
                default = 0
            },
            {
                name = "b2",
                type = "SPFLOAT",
                description = "biquad coefficient.",
                default = 0
            },
            {
                name = "a0",
                type = "SPFLOAT",
                description = "biquad coefficient.",
                default = 0
            },
            {
                name = "a1",
                type = "SPFLOAT",
                description = "biquad coefficient.",
                default = 0
            },
            {
                name = "a2",
                type = "SPFLOAT",
                description = "biquad coefficient.",
                default = 0
            }
        }
    },

    modtype = "module",

    description = [[A sweepable biquadratic general purpose filter

    ]],

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

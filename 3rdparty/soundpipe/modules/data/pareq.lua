sptbl["pareq"] = {

    files = {
        module = "pareq.c",
        header = "pareq.h",
        example = "ex_pareq.c",
    },

    func = {
        create = "sp_pareq_create",
        destroy = "sp_pareq_destroy",
        init = "sp_pareq_init",
        compute = "sp_pareq_compute",
    },

    params = {
        optional = {
            {
                name = "fc",
                type = "SPFLOAT",
                description = "Center frequency in peak mode, corner frequency in shelving mode.",
                default = 1000
            },
            {
                name = "v",
                type = "SPFLOAT",
                description ="Amount at which the center frequency value shall be increased or decreased. A value of 1 is a flat response.",
                default = 1
            },
            {
                name = "q",
                type = "SPFLOAT",
                description ="Q of the filter. sqrt(0.5) is no resonance.",
                default = 0.707
            },
            {
                name = "mode",
                type = "SPFLOAT",
                description ="EQ mode. 0 = peak, 1 = low shelving, 2 = high shelving",
                default = 0
            },
        }
    },

    modtype = "module",

    description = [[Parametric Equalizer
This is an implementation of Zoelzer's parametric equalizer filter.]],

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

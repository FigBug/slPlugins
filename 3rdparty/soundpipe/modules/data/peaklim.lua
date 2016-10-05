sptbl["peaklim"] = {

    files = {
        module = "peaklim.c",
        header = "peaklim.h",
        example = "ex_peaklim.c",
    },

    func = {
        create = "sp_peaklim_create",
        destroy = "sp_peaklim_destroy",
        init = "sp_peaklim_init",
        compute = "sp_peaklim_compute",
    },

    params = {
        optional = {
            {
                name = "atk",
                type = "SPFLOAT",
                description = "Attack time, in seconds",
                default = 0.01
            },
            {
                name = "rel",
                type = "SPFLOAT",
                description ="Release time, in seconds",
                default = 0.1
            },
            {
                name = "thresh",
                type = "SPFLOAT",
                description ="Threshold, in dB",
                default = 0
            },
        }
    },

    modtype = "module",

    description = [[Peak limiter 
This is a simple peak limiting algorithm, based off code from the Stanford
Music-424 class.
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "input",
            description = "Input signal."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Output signal."
        },
    }

}

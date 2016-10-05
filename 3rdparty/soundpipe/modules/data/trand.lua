sptbl["trand"] = {

    files = {
        module = "trand.c",
        header = "trand.h",
        example = "ex_trand.c",
    },

    func = {
        create = "sp_trand_create",
        destroy = "sp_trand_destroy",
        init = "sp_trand_init",
        compute = "sp_trand_compute",
    },

    params = {
        optional = {
            {
                name = "min",
                type = "SPFLOAT",
                description ="Minimum value to use.",
                default = 0
            },
            {
                name = "max",
                type = "SPFLOAT",
                description ="Maximum value to use.",
                default = 1
            },
        }
    },

    modtype = "module",

    description = [[Triggered random number generator.
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "When non-zero, will create a new random value."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

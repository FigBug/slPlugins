sptbl["tgate"] = {

    files = {
        module = "tgate.c",
        header = "tgate.h",
        example = "ex_tgate.c",
    },

    func = {
        create = "sp_tgate_create",
        destroy = "sp_tgate_destroy",
        init = "sp_tgate_init",
        compute = "sp_tgate_compute",
    },

    params = {
        optional = {
            {
                name = "time",
                type = "SPFLOAT",
                description = "Duration of the gate (in seconds)",
                default = 0.5
            },
        }
    },

    modtype = "module",

    description = [[A triggerable gate.
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trigger",
            description = "When non-zero, will (re)trigger gate."
        },
    },

    outputs = {
        {
            name = "gate",
            description = "Gate output."
        },
    }

}

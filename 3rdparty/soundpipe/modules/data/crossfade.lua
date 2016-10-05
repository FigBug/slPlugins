sptbl["crossfade"] = {

    files = {
        module = "crossfade.c",
        header = "crossfade.h",
        example = "ex_crossfade.c",
    },

    func = {
        create = "sp_crossfade_create",
        destroy = "sp_crossfade_destroy",
        init = "sp_crossfade_init",
        compute = "sp_crossfade_compute",
    },

    params = {
        mandatory = {
        },

        optional = {
            {
                name = "pos",
                type = "SPFLOAT",
                description = "Crossfade position. 0 = all signal 1, 1 = all signal 2",
                default = 0.5
            },
        }
    },

    modtype = "module",

    description = [[Crossfade two signals. 
This module will perform a linear crossfade between two input signals.
]],

    ninputs = 2,
    noutputs = 1,

    inputs = {
        {
            name = "in1",
            description = "Input signal 1."
        },
        {
            name = "in2",
            description = "Input signal 2."
        },
    },

    outputs = {
        {
            name = "out",
            description = "signal output."
        },
    }

}

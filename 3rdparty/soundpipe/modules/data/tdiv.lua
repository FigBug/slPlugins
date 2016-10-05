sptbl["tdiv"] = {

    files = {
        module = "tdiv.c",
        header = "tdiv.h",
        example = "ex_tdiv.c",
    },

    func = {
        create = "sp_tdiv_create",
        destroy = "sp_tdiv_destroy",
        init = "sp_tdiv_init",
    },

    params = {
        optional = {
            {
                name = "num",
                type = "SPFLOAT",
                description = "Triggers every N times.",
                default = 2
            },
            {
                name = "offset",
                type = "SPFLOAT",
                description = "Offset amoutn.",
                default = 0
            },
        }
    },

    modtype = "module",

    description = [[Trigger divider.
This module will take in a trigger signal, and output a trigger signal
every N times.

For instance, when N = 3:

in: * * * * * * * * *

out *     *     *   

]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trigger",
            description = "Trigger. Should be a non-zero value"
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

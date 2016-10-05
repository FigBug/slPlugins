sptbl["ptrack"] = {

    files = {
        module = "ptrack.c",
        header = "ptrack.h",
        example = "ex_ptrack.c",
    },

    func = {
        create = "sp_ptrack_create",
        destroy = "sp_ptrack_destroy",
        init = "sp_ptrack_init",
        compute = "sp_ptrack_compute",
    },

    params = {
        mandatory = {
            {
                name = "ihopsize",
                type = "int",
                description = "hop size.",
                default = 512
            },
            {
                name = "ipeaks",
                type = "int",
                description = "Number of peaks.",
                default = 20
            }
        },

        optional = {
        }
    },

    modtype = "module",

    description = [[Track pitch of a signal.
Ptrack is a pitch tracker, based on an algorithm originally created by
Miller Puckette.
]],

    ninputs = 1,
    noutputs = 2,

    inputs = {
        {
            name = "in",
            description = "Input signal."
        },
    },

    outputs = {
        {
            name = "freq",
            description = "Guessed frequency (in Hz)."
        },
        {
            name = "amp",
            description = "Guessed amplitude."
        },
    }

}

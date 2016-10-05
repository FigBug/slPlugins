sptbl["tseg"] = {

    files = {
        module = "tseg.c",
        header = "tseg.h",
        example = "ex_tseg.c",
    },

    func = {
        create = "sp_tseg_create",
        destroy = "sp_tseg_destroy",
        init = "sp_tseg_init",
        compute = "sp_tseg_compute",
    },

    params = {
        mandatory = {
            {
                name = "ibeg",
                type = "SPFLOAT",
                description = "Beginning value.",
                default = 0.001; 
            },
        },

        optional = {
            {
                name = "end",
                type = "SPFLOAT",
                description = "End parameter to go to.",
                default = 1.0
            },
            {
                name = "dur",
                type = "SPFLOAT",
                description = "duration to rise to (in seconds).",
                default = 1.0
            },
            {
                name = "type",
                type = "SPFLOAT",
                description ="The type of line, which determines slope of line",
                default = -1.0
            },
        }
    },

    modtype = "module",

    description = [[This module creates a series of line segments. 
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "trigger."
        },
    },

    outputs = {
        {
            name = "out",
            description = "tseg output."
        },
    }

}

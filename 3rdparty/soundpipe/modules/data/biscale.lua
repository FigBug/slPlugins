sptbl["biscale"] = {

    files = {
        module = "biscale.c",
        header = "biscale.h",
        example = "ex_biscale.c",
    },

    func = {
        create = "sp_biscale_create",
        destroy = "sp_biscale_destroy",
        init = "sp_biscale_init",
        compute = "sp_biscale_compute",
    },

    params = {
        optional = {
            {
                name = "min",
                type = "SPFLOAT",
                description = "Minimum value to scale to.",
                default = 0
            },
            {
                name = "max",
                type = "SPFLOAT",
                description ="Maximum value to scale to.",
                default = 1
            },
        }
    },

    modtype = "module",

    description = [[Bipolar Scale

    This module scales from bipolar [-1, 1] to another range defined by min and max.
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "in",
            description = "Signal to be scaled."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Scaled signal out"
        },
    }

}

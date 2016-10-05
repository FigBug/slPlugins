sptbl["fold"] = {

    files = {
        module = "fold.c",
        header = "fold.h",
        example = "ex_fold.c",
    },

    func = {
        create = "sp_fold_create",
        destroy = "sp_fold_destroy",
        init = "sp_fold_init",
        compute = "sp_fold_compute",
    },

    params = {
        optional = {
            {
                name = "incr",
                type = "SPFLOAT",
                description = "Increment",
                default = 1000
            },
        }
    },

    modtype = "module",

    description = [[Adds artificial foldover to an audio signal

    Primarily created for use with Decimator.]],

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

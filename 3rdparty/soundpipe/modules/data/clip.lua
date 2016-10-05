sptbl["clip"] = {

    files = {
        module = "clip.c",
        header = "clip.h",
        example = "ex_clip.c",
    },

    func = {
        create = "sp_clip_create",
        destroy = "sp_clip_destroy",
        init = "sp_clip_init",
        compute = "sp_clip_compute",
    },

    params = {
        optional = {
            {
                name = "lim",
                type = "SPFLOAT",
                description = "threshold / limiting value.",
                default = 1.0
            },
        }
    },

    modtype = "module",

    description = [[Applies clip-limiting to a signal]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "in",
            description = "Input signal."
        }
    },

    outputs = {
        {
            name = "out",
            description = "Output signal."
        }
    }

}

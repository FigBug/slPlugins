sptbl["pinknoise"] = {

    files = {
        module = "pinknoise.c",
        header = "pinknoise.h",
        example = "ex_pinknoise.c",
    },

    func = {
        create = "sp_pinknoise_create",
        destroy = "sp_pinknoise_destroy",
        init = "sp_pinknoise_init",
        compute = "sp_pinknoise_compute",
    },

    params = {
        optional = {
            {
                name = "amp",
                type = "SPFLOAT",
                description = "Amplitude. (Value between 0-1).",
                default = 1.0
            },
        }
    },

    modtype = "module",

    description = [[Pink pinknoise generator]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
        {
            name = "dummy",
            description = "This doesn't do anything"
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

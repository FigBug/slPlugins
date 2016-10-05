sptbl["pan2"] = {

    files = {
        module = "pan2.c",
        header = "pan2.h",
        example = "ex_pan2.c",
    },

    func = {
        create = "sp_pan2_create",
        destroy = "sp_pan2_destroy",
        init = "sp_pan2_init",
        compute = "sp_pan2_compute",
    },

    params = {
        mandatory = {
        },

        optional = {
            {
                name = "type",
                type = "uint32_t",
                description = [[Panning type. 0 = equal power, 1 = square root, 2 = linear,
3 = alternative equal power. Values outside this range will wrap. ]],
                default = 0
            },
            {
                name = "pan",
                type = "SPFLOAT",
                description ="Panning. A value of -1 is hard left, and a value of 1 is hard right, and 0 is center.",
                default = 0
            },
        }
    },

    modtype = "module",

    description = [[Panner]],

    ninputs = 1,
    noutputs = 2,
    
    inputs = {
        {
            name = "in",
            description = "Mono input signal to be panned."
        },
    },

    outputs = {
        {
            name = "out_left",
            description = "Stereo left output."
        },
        {
            name = "out_right",
            description = "Stereo right output."
        },
    }

}

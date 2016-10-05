sptbl["panst"] = {

    files = {
        module = "panst.c",
        header = "panst.h",
        example = "ex_panst.c",
    },

    func = {
        create = "sp_panst_create",
        destroy = "sp_panst_destroy",
        init = "sp_panst_init",
        compute = "sp_panst_compute",
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
                description ="Panning. A value of -1 is hard left, and a value of 1 is hard right.",
                default = 0
            },
        }
    },

    modtype = "module",

    description = [[Stereo Panner]],

    ninputs = 2,
    noutputs = 2,

    inputs = {
        {
            name = "in2",
            description = "Stereo left input signal."
        },
        {
            name = "in2",
            description = "Stereo right input signal."
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

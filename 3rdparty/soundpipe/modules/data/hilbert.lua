sptbl["hilbert"] = {

    files = {
        module = "hilbert.c",
        header = "hilbert.h",
        example = "ex_hilbert.c",
    },

    func = {
        create = "sp_hilbert_create",
        destroy = "sp_hilbert_destroy",
        init = "sp_hilbert_init",
        compute = "sp_hilbert_compute",
    },

    params = {
    },

    modtype = "module",

    description = [[Hilbert transform

]],

    ninputs = 1,
    noutputs = 2,

    inputs = {
        {
            name = "input",
            description = "Input signal."
        },
    },

    outputs = {
        {
            name = "out1",
            description = "Sine output."
        },
        {
            name = "out2",
            description = "Cosine output."
        },
    }

}

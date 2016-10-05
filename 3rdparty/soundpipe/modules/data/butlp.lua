sptbl["butlp"] = {

    files = {
        module = "butlp.c",
        header = "butlp.h",
        example = "ex_butlp.c",
    },

    func = {
        create = "sp_butlp_create",
        destroy = "sp_butlp_destroy",
        init = "sp_butlp_init",
        compute = "sp_butlp_compute",
    },

    params = {
        optional = {
            {
                name = "freq",
                type = "SPFLOAT",
                description = "Cutoff Frequency.",
                default = 1000
            },
        }
    },

    modtype = "module",

    description = [[Lowpass Butterworth filter]],

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
            name = "output",
            description = "Signal output."
        },
    }

}

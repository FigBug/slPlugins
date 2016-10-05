sptbl["butbp"] = {

    files = {
        module = "butbp.c",
        header = "butbp.h",
        example = "ex_butbp.c",
    },

    func = {
        create = "sp_butbp_create",
        destroy = "sp_butbp_destroy",
        init = "sp_butbp_init",
        compute = "sp_butbp_compute",
    },

    params = {
        optional = {
            {
                name = "freq",
                type = "SPFLOAT",
                description = "Center Frequency. (in Hertz)",
                default = 1000
            },
            {
                name = "bw",
                type = "SPFLOAT",
                description = "Bandwidth. (in Hertz)",
                default = 10
            },
        }
    },

    modtype = "module",

    description = [[Bandpass Butterworth filter]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "input",
            description = "Input signal that the filter should be applied to."
        },
    },

    outputs = {
        {
            name = "output",
            description = "Output signal (input modified by the filter)."
        },
    }

}

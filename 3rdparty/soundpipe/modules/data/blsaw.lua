sptbl["blsaw"] = {

    files = {
        module = "blsaw.c",
        header = "blsaw.h",
        example = "ex_blsaw.c",
    },

    func = {
        create = "sp_blsaw_create",
        destroy = "sp_blsaw_destroy",
        init = "sp_blsaw_init",
        compute = "sp_blsaw_compute",
    },

    params = {
        optional = {
            {
                name = "freq",
                type = "SPFLOAT",
                description = "Frequency, (range 0-20000)",
                default = 440
            },
            {
                name = "amp",
                type = "SPFLOAT",
                description ="Amplitude (range 0-1).",
		default = 1.0
            },
        }
    },

    modtype = "module",

    description = [[Bandlimited sawtooth oscillator
This is a bandlimited sawtooth oscillator ported from the "sawtooth" function from the Faust
programming language.
]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
        {
            name = "dummy",
            description = "This doesn't do anything."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

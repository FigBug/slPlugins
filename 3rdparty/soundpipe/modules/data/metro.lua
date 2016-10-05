sptbl["metro"] = {

    files = {
        module = "metro.c",
        header = "metro.h",
        --example = "ex_tone.c",
    },

    func = {
        create = "sp_metro_create",
        destroy = "sp_metro_destroy",
        init = "sp_metro_init",
        compute = "sp_metro_compute",
    },

    params = {
        optional = {
             {
                name = "freq",
                type = "SPFLOAT",
                description = "The frequency to repeat.",
                default = 2.0
            },
        }
    },

    modtype = "module",

    description = [[Metronome

    Metro produces a series of 1-sample ticks at a regular rate. Typically, this is used alongside trigger-driven modules.]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
        {
            name = "dummy",
            description = "This input doesn't do anything"
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal out."
        },
    }

}

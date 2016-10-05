sptbl["pdhalf"] = {

    files = {
        module = "pdhalf.c",
        header = "pdhalf.h",
        example = "ex_pdhalf.c",
    },

    func = {
        create = "sp_pdhalf_create",
        destroy = "sp_pdhalf_destroy",
        init = "sp_pdhalf_init",
        compute = "sp_pdhalf_compute",
    },

    params = {
        optional = {
            {
                name = "amount",
                type = "SPFLOAT",
                description = "Amount of distortion, within the range [-1, 1]. 0 is no distortion.",
                default = 0
            },
        }
    },

    modtype = "module",

    description = [[Casio-style phase distortion with "pivot point" on the X axis
This module is designed to emulate the classic phase distortion synthesis technique.
From the mid 90's. The technique reads the first and second halves of the ftbl
at different rates in order to warp the waveform. For example, pdhalf can 
smoothly transition a sinewave into something approximating a sawtooth wave.
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "sig",
            description = "Input signal, typically a phasor normalize 0-1."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

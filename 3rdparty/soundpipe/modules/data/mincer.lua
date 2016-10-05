sptbl["mincer"] = {

    files = {
        module = "mincer.c",
        header = "mincer.h",
        example = "ex_mincer.c",
    },

    func = {
        create = "sp_mincer_create",
        destroy = "sp_mincer_destroy",
        init = "sp_mincer_init",
        compute = "sp_mincer_compute",
    },

    params = {
        mandatory = {
            {
                name = "ft",
                type = "sp_ftbl *",
                description = "ftable containing an audio file.",
                default = "N/A"
            }
        },

        optional = {
            {
                name = "time",
                type = "SPFLOAT",
                description = "Position in time. When non-changing it will do a spectral freeze of a the current point in time.",
                default = 0
            },
            {
                name = "amp",
                type = "SPFLOAT",
                description ="Amplitude.",
                default = 1
            },
            {
                name = "pitch",
                type = "SPFLOAT",
                description ="Pitch ratio. A value of. 1  normal, 2 is double speed, 0.5 is halfspeed, etc.",
                default = 1
            },
        }
    },

    modtype = "module",

    description = [[Phase-locked vocoder.

    Mincer is a phase-locked vocoder. It has the ability to play back an audio 
file loaded into an ftable like a sampler would. Unlike a typical sampler, mincer allows
time and pitch to be controlled separately. 
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

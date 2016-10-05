sptbl["pshift"] = {

    files = {
        module = "pshift.c",
        header = "pshift.h",
        example = "ex_pshift.c",
    },

    func = {
        create = "sp_pshift_create",
        destroy = "sp_pshift_destroy",
        init = "sp_pshift_init",
        compute = "sp_pshift_compute",
    },

    params = {
        optional = {
            {
                name = "shift",
                type = "SPFLOAT",
                description = "Pitch shift (in semitones), range -24/24.",
                default = 0
            },
            {
                name = "window",
                type = "SPFLOAT",
                description ="Window size (in samples), max 10000",
                default = 1000
            },
            {
                name = "xfade",
                type = "SPFLOAT",
                description ="Crossfade (in samples), max 10000",
                default = 10
            },
        }
    },

    modtype = "module",

    description = [[Time-domain pitch shifter.]],

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
            name = "out",
            description = "Signal output."
        },
    }

}

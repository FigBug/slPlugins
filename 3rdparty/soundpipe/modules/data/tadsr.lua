sptbl["tadsr"] = {

    files = {
        module = "tadsr.c",
        header = "tadsr.h",
        example = "ex_tadsr.c",
    },

    func = {
        create = "sp_tadsr_create",
        destroy = "sp_tadsr_destroy",
        init = "sp_tadsr_init",
        compute = "sp_tadsr_compute",
    },

    params = {
        optional = {
            {
                name = "atk",
                type = "SPFLOAT",
                description = "Attack time",
                default = 0.1
            },
            {
                name = "dec",
                type = "SPFLOAT",
                description ="Decay time",
                default = 0.1
            },
            {
                name = "sus",
                type = "SPFLOAT",
                description ="Sustain Level",
                default = 0.5
            },
            {
                name = "rel",
                type = "SPFLOAT",
                description ="release",
                default = 0.5
            },
        }
    },

    modtype = "module",

    description = [[Triggerable classic ADSR envelope]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "Expects trigger signal."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Envelope signal output."
        },
    }

}

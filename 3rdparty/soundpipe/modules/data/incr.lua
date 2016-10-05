sptbl["incr"] = {

    files = {
        module = "incr.c",
        header = "incr.h",
        example = "ex_incr.c",
    },

    func = {
        create = "sp_incr_create",
        destroy = "sp_incr_destroy",
        init = "sp_incr_init",
        compute = "sp_incr_compute",
    },

    params = {
        mandatory = {
            {
                name = "val",
                type = "SPFLOAT",
                description = "Initial value",
                default = 0.5
            },
        },
        optional = {
            {
                name = "step",
                type = "SPFLOAT",
                description = "Step value to increment by.",
                default = 0.1
            },
            {
                name = "min",
                type = "SPFLOAT",
                description = "Minimum value",
                default = 0
            },
            {
                name = "max",
                type = "SPFLOAT",
                description = "Maximum value",
                default = 1
            },
        }
    },

    modtype = "module",

    description = [[Trigger-based Incrementor
When triggered, this module will increment and decrement a value bounded between a min
and max. Initially, this was designed for the specific use case of interfacing with the
griffin knob. 
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "Trigger signal. When positive, the value will increase. When negative, the value will decrease."
        },
    },

    outputs = {
        {
            name = "out",
            description = "incr output."
        },
    }

}

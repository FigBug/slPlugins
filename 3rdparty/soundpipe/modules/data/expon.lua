sptbl["expon"] = {

    files = {
        module = "expon.c",
        header = "expon.h",
        example = "ex_expon.c",
    },

    func = {
        create = "sp_expon_create",
        destroy = "sp_expon_destroy",
        init = "sp_expon_init",
        compute = "sp_expon_compute",
    },

    params = {
        optional = {
            {
                name = "a",
                type = "SPFLOAT",
                description = "Inital point.",
                default = 1.0
            },
            {
                name = "dur",
                type = "SPFLOAT",
                description = "Duration (in seconds)",
                default = 1.0
            },
            {
                name = "b",
                type = "SPFLOAT",
                description = "End point",
                default = 1.0
            }
        },

    },

    modtype = "module",

    description = [[Produce a line segment with exponential slope
This will generate a line from value A to value B in given amount of time. 
When it reaches it's target, it will stay at that value. 
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "When nonzero, will retrigger line segment"
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

sptbl["line"] = {

    files = {
        module = "line.c",
        header = "line.h",
        example = "ex_line.c",
    },

    func = {
        create = "sp_line_create",
        destroy = "sp_line_destroy",
        init = "sp_line_init",
        compute = "sp_line_compute",
    },

    params = {
        optional = {
            {
                name = "a",
                type = "SPFLOAT",
                description = "Inital point.",
                default = 0.0
            },
            {
                name = "dur",
                type = "SPFLOAT",
                description = "Duration (in seconds)",
                default = 0.5
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

    description = [[Produce a line segment with linear slope
This will generate a line from value A to value B in given amount of time. 
When it reaches it's target, it will stay at that value. 
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "When non-zero, will retrigger the line slope."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

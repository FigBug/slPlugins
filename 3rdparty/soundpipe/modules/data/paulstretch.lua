sptbl["paulstretch"] = {

    files = {
        module = "paulstretch.c",
        header = "paulstretch.h",
        example = "ex_paulstretch.c",
    },

    func = {
        create = "sp_paulstretch_create",
        destroy = "sp_paulstretch_destroy",
        init = "sp_paulstretch_init",
        compute = "sp_paulstretch_compute",
    },

    params = {
        mandatory = {
            {
                name = "ft",
                type = "sp_ftbl *",
                description = "ftable containing audio data",
                default = "N/A"
            },
            {
                name = "windowsize",
                type = "SPFLOAT",
                description = "Window size, in seconds.",
                default = 10
            },
            {
                name = "stretch",
                type = "SPFLOAT",
                description = "Stretch factor, 1.0 is no stretch.",
                default = 10
            },
        },

        optional = {
        }
    },

    modtype = "module",

    description = [[An extreme time-stretching algorithm by Paul Nasca Octavian
This is an implementation of the popular paulstretch algorithm used for time
stretching an audio signal to create ambient textures. Ideally, this algorithm 
is best used for stretching signals by very very long amounts. 

This version of paulstretch will take an ftable and loop through it, make
it an ideal means for creating sustained pads. 
]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

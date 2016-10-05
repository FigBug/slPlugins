sptbl["pitchamdf"] = {

    files = {
        module = "pitchamdf.c",
        header = "pitchamdf.h",
        example = "ex_pitchamdf.c",
    },

    func = {
        create = "sp_pitchamdf_create",
        destroy = "sp_pitchamdf_destroy",
        init = "sp_pitchamdf_init",
        compute = "sp_pitchamdf_compute",
    },

    params = {
        mandatory = {
            {
                name = "min",
                type = "SPFLOAT",
                description = "Minimum expected frequency to detect",
                default = 200
            },
            {
                name = "max",
                type = "SPFLOAT",
                description = "Maximum expected frequency to detect",
                default = 500
            }
        }
    },

    modtype = "module",

    description = [[Pitch detection using AMDF method.
Pitchamdf tracks the pitch of signal using the AMDF (Average Magnitude Difference Function) method of 
Pitch following. 
]],

    ninputs = 1,
    noutputs = 2,

    inputs = {
        {
            name = "input",
            description = "Signal input"
        },
    },

    outputs = {
        {
            name = "freq",
            description = "Detected frequency."
        },
        {
            name = "rms",
            description = "Average amplitude."
        },
    }

}

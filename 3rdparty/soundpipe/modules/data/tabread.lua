sptbl["tabread"] = {
    files = {
        module = "tabread.c",
        header = "tabread.h",
        example = "ex_tabread.c",
    },

    func = {
        create = "sp_tabread_create",
        destroy = "sp_tabread_destroy",
        init = "sp_tabread_init",
        compute = "sp_tabread_compute",
    },

    params = {
        mandatory = {
            {
                name = "ft",
                type = "sp_ftbl *",
                description = "A properly allocated table (using a function like sp_gen_file).",
                default = "N/A"
            },
            {
                name = "mode",
                type = "SPFLOAT",
                description ="1 = scaled index, 0 = unscaled index",
                default = 1.0
            },
        },

        optional = {
            {
                name = "index",
                type = "SPFLOAT",
                description ="index position, either scaled or unscaled with mode parameter",
                default = 0
            },
            {
                name = "offset",
                type = "SPFLOAT",
                description ="Offset from beginning of ftable. If the mode is scaled, then it is in range 0-1, other wise it is the index position.",
                default = 1.0
            },
            {
                name = "wrap",
                type = "SPFLOAT",
                description ="Enable wraparound. 1 = on; 0 = 0ff.",
                default = 1.0
            }
        }
    },

    modtype = "module",

    description = [[Table 
        Read through an sp_ftbl with linear interpolation.
    ]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
        {
            name = "in",
            description = "Signal in."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }
}

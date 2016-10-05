sptbl["bar"] = {

    files = {
        module = "bar.c",
        header = "bar.h",
        example = "ex_bar.c",
    },

    func = {
        create = "sp_bar_create",
        destroy = "sp_bar_destroy",
        init = "sp_bar_init",
        compute = "sp_bar_compute",
    },

    params = {
        mandatory = {
            {
                name = "iK",
                type = "SPFLOAT",
                description = "Dimensionless stiffness parameter",
                default = 3
            },
            {
                name = "ib",
                type = "SPFLOAT",
                description = "High-frequency loss parameter. Keep this small",
                default = 0.001
            }
        },

        optional = {
            {
                name = "bcL",
                type = "SPFLOAT",
                description = "Boundary condition at left end of bar. 1 = clamped, 2 = pivoting, 3 = free",
                default = 1
            },
            {
                name = "bcR",
                type = "SPFLOAT",
                description = "Boundary condition at right end of bar. 1 = clamped, 2 = pivoting, 3 = free",
                default = 1
            },
            {
                name = "T30",
                type = "SPFLOAT",
                description ="30db decay time (in seconds).",
                default = 3
            },
            {
                name = "scan",
                type = "SPFLOAT",
                description ="Speed of scanning the output location.",
                default = 0.25
            },
            {
                name = "pos",
                type = "SPFLOAT",
                description ="Position along bar that strike occurs.",
                default = 0.2
            },
            {
                name = "vel",
                type = "SPFLOAT",
                description ="Normalized strike velocity",
                default = 500
            },
            {
                name = "wid",
                type = "SPFLOAT",
                description ="Spatial width of strike.",
                default = 0.05
            },
        }
    },

    modtype = "module",

    description = [[Physical model approximating the sound of a struck metal bar]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "Non-zero values will strike the bar."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
    }

}

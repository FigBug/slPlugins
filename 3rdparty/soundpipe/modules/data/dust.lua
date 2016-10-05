sptbl["dust"] = {

    files = {
        module = "dust.c",
        header = "dust.h",
        example = "ex_dust.c",
    },

    func = {
        create = "sp_dust_create",
        destroy = "sp_dust_destroy",
        init = "sp_dust_init",
        compute = "sp_dust_compute",
    },

    params = {
        optional = {
            {
                name = "amp",
                type = "SPFLOAT",
                description = "",
                default = 0.3
            },
            {
                name = "density",
                type = "SPFLOAT",
                description = "",
                default = 10
            },
            {
                name = "bipolar",
                type = "int",
                description = "Bipolar flag. A non-zero makes the signal bipolar as opposed to unipolar. ",
                default = 0
            }
        }
    },

    modtype = "module",

    description = [[A series of random impulses]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
    },

    outputs = {
        {
            name = "out",
            description = "Signal out."
        },
    }

}

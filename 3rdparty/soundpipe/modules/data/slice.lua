sptbl["slice"] = {

    files = {
        module = "slice.c",
        header = "slice.h",
        example = "ex_slice.c",
    },

    func = {
        create = "sp_slice_create",
        destroy = "sp_slice_destroy",
        init = "sp_slice_init",
        compute = "sp_slice_compute",
    },

    params = {
        mandatory = {
            {
                name = "vals",
                type = "sp_ftbl *",
                description = "A table containing slice points, in samples",
                default = "N/A"
            },
            {
                name = "buf",
                type = "sp_ftbl *",
                description = "The buffer containing the audio samples.",
                default = "N/A"
            }
        },

        optional = {
            {
                name = "id",
                type = "SPFLOAT",
                description = "Value position.",
                default = 0
            },
        }
    },

    modtype = "module",

    description = [[Yet another slice-based sample player.
This module takes in an audio buffer and a table with slice points. 
]],

    ninputs = 1,
    noutputs = 1,

    inputs = {
        {
            name = "trig",
            description = "When non-zero, (re)triggers samples"
        },
    },

    outputs = {
        {
            name = "out",
            description = "Signal output."
        },
        {
            name = "out_right",
            description = "Stereo right output for slice."
        },
    }

}

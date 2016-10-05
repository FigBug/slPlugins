sptbl["tblrec"] = {

    files = {
        module = "tblrec.c",
        header = "tblrec.h",
        example = "ex_tblrec.c",
    },

    func = {
        create = "sp_tblrec_create",
        destroy = "sp_tblrec_destroy",
        init = "sp_tblrec_init",
        compute = "sp_tblrec_compute",
    },

    params = {
        mandatory = {
            {
                name = "bar",
                type = "sp_ftbl *",
                description = "",
                default = "N/A"
            },
        },
    },

    modtype = "module",

    description = [[Record a signal to an ftable.
This module will write audio-rate signals to a preallocated soundpipe ftable. 
Every time record is enabled, it will got back to index 0 overwrite any
previous information that was on it. 
]],

    ninputs = 2,
    noutputs = 0,

    inputs = {
        {
            name = "in",
            description = "Input signal."
        },
        {
            name = "trig",
            description = "When non-zero, will toggle recording on/off. Recording will set the playhead back to zero"
        },
    },

    outputs = {
        {
            name = "out",
            description = "This doesn't do anything. should be NULL"
        }
    }

}

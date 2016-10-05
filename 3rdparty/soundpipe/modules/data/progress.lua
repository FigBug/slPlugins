sptbl["progress"] = {

    files = {
        module = "progress.c",
        header = "progress.h",
        example = "ex_progress.c",
    },

    func = {
        create = "sp_progress_create",
        destroy = "sp_progress_destroy",
        init = "sp_progress_init",
        compute = "sp_progress_compute",
    },

    params = {
        optional = {
            {
                name = "nbars",
                type = "int",
                description = "",
                default = 30
            },
            {
                name = "skip",
                type = "int",
                description ="How many samples to skip before checking",
                default = 1000
            },
        }
    },

    modtype = "module",

    description = [[A simple progress bar for the commandline

    Mostly ideal for offline renderings and programs with finite length. Escape characters are used to show/hide the cursor. Interruption before finishing may cause the cursor to disappear.]],

    ninputs = 0,
    noutputs = 0,

    inputs = {
        {
            name = "dummy",
            description = "This doesn't do anything."
        },
    },

    outputs = {
        {
            name = "dummy",
            description = "This doesn't do anything."
        },
    }

}

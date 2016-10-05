sptbl["gen_composite"] = {

    files = {
        module = "ftbl.c",
        header = "ftbl.h",
        example = "ex_gen_composite.c",
    },

    func = {
        name = "sp_gen_composite",
    },

    params = {
        {
            name = "argstring",
            type = "char *",
            description = [[
a string of space-separated parameters, in groups of four:

arg 1 is the partial number. must be positive, but it doesn't need to be a whole number.

arg 2 is the strength.

arg 3 is the initial phase (expressed in degrees)

arg 4 is the dc offset. A dc offset of 2 will put a 2-strength sinusoid in the range
from (-2,2) to (0, 4)
]],
            default = "0.5 0.5 270 0.5"
        },
    },

    modtype = "gen",

    description = [[Generate a composite waveform of sinusoids]],

}

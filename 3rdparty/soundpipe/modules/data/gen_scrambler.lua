sptbl["gen_scrambler"] = {

    files = {
        module = "ftbl.c",
        header = "ftbl.h",
        example = "ex_scrambler.c",
    },

    func = {
        name = "sp_gen_scrambler",
    },

    params = {
        {
            name = "src",
            type = "sp_ftbl *",
            description = [[source ftable]],
            default = 123456
        },
        {
            name = "dest",
            type = "sp_ftbl **",
            description = [[destination ftable]],
            default = 123456
        },
    },

    modtype = "gen",

    description = [[Scrambles phase of ftable.
This gen routine will copy the ftable, apply an FFT, apply
a random phase, and then do an inverse FFT. This effect 
is ideal for creating pad-like sounds. 
]],

}

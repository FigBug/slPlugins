sptbl["padsynth"] = {

    files = {
        module = "ftbl.c",
        header = "ftbl.h",
        example = "extra/ex_padsynth.c",
    },

    func = {
        name = "sp_gen_padsynth",
    },

    params = {
        {
            name = "amps",
            type = "sp_ftbl *",
            description = [[ftable of amplitudes to use]],
            default = "N/A"
        },
        {
            name = "f",
            type = "SPFLOAT",
            description = [[Base frequency.]],
            default = 440.0
        },
        {
            name = "bw",
            type = "SPFLOAT",
            description = [[Bandwidth.]],
            default = 40.0
        },
    },

    modtype = "gen",

    description = [[An implementation of the Padsynth Algorithm by Paul Nasca. 

This is a basic implemenation of the padsynth algorithm. More information can be found here:

http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm

This gen routine requires libfftw, and is not compiled by default. See config.mk for more info.
    
    ]],

}

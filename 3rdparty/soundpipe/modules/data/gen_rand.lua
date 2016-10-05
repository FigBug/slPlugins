sptbl["gen_rand"] = {

    files = {
        module = "ftbl.c",
        header = "ftbl.h",
        example = "ex_gen_rand.c",
    },

    func = {
        name = "sp_gen_rand",
    },

    params = {
        {
            name = "argstring",
            type = "char *",
            description = [[A string of value pairs. The first value is the value, then the probability.]],
            default = "1 0.5 2 0.5"
        },
    },

    modtype = "gen",

    description = [[Generates a user defined random number distribution.]],

}

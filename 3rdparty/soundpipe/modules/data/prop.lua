sptbl["prop"] = {

    files = {
        module = "prop.c",
        header = "prop.h",
        example = "ex_prop.c",
    },

    func = {
        create = "sp_prop_create",
        destroy = "sp_prop_destroy",
        init = "sp_prop_init",
        compute = "sp_prop_compute",
        other = {
            sp_prop_reset = {
                description = "Resets prop back to starting position.",
                args = {
                }
            }
        }
    },

    params = {
        mandatory = {
            {
                name = "str",
                type = "const char *",
                description = "Prop string to be parsed.",
                default = "N/A"
            },
        },

        optional = {
            {
                name = "bpm",
                type = "SPFLOAT",
                description = "Beats per minute of the prop string.",
                default = 60
            },
        }
    },

    modtype = "module",

    description = [[Simple rhythmic notation gate generator

    Creates a gate using a simple rhythmic notation system called prop. When it reaches the end of the prop string, it will loop back to the beginning.

Prop has a few basic rules:

1. A '+' denotes a on. A '-' denotes an off (rest). They each have an equal duration of a quarter note.

2. On and off values can be strung together to create equally spaced gates: +-+--

3. When notes are enclosed in parantheses '()' following a positive integer N, their duration is reduced N times: ++2(+-)

4. When notes are enclosed in brackets '[]' following a positive integer N, their duration is scaled by a factor of N: ++2[++]

5. Parenthesis and brackets can be nested: +- 2[3(+2(++)+)]2(++)

]],

    ninputs = 0,
    noutputs = 1,

    inputs = {
        {
            name = "dummy",
            description = "This is doesn't do anything."
        },
    },

    outputs = {
        {
            name = "out",
            description = "Gate output."
        },
    }

}

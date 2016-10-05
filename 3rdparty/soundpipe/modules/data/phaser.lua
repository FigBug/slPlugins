sptbl["phaser"] = {

    files = {
        module = "phaser.c",
        header = "phaser.h",
        example = "ex_phaser.c",
    },

    func = {
        create = "sp_phaser_create",
        destroy = "sp_phaser_destroy",
        init = "sp_phaser_init",
        compute = "sp_phaser_compute",
    },

    params = {
        optional = {
            {
                name = "MaxNotch1Freq",
                type = "SPFLOAT",
                description = "Between 20 and 10000",
                default = 800 
            },
            {
                name = "MinNotch1Freq",
                type = "SPFLOAT",
                description ="Between 20 and 5000",
                default = 100
            },
            {
                name = "Notch_width",
                type = "SPFLOAT",
                description ="Between 10 and 5000",
                default = 1000
            },
            {
                name = "NotchFreq",
                type = "SPFLOAT",
                description ="Between 1.1 and 4",
                default = 1.5 
            },
            {
                name = "VibratoMode",
                type = "SPFLOAT",
                description ="1 or 0",
                default = 1
            },
            {
                name = "depth",
                type = "SPFLOAT",
                description ="Between 0 and 1",
                default = 1
            },
            {
                name = "feedback_gain",
                type = "SPFLOAT",
                description ="Between 0 and 1",
                default = 0
            },
            {
                name = "invert",
                type = "SPFLOAT",
                description ="1 or 0",
                default = 0
            },
            {
                name = "level",
                type = "SPFLOAT",
                description ="-60 to 10 dB",
                default = 0
            },
            {
                name = "lfobpm",
                type = "SPFLOAT",
                description ="Between 24 and 360",
                default = 30
            },
        }
    },

    modtype = "module",

    description = [[A stereo phaser

	This is a stereo phaser, generated from Faust code taken from the 
Guitarix project.
]],

    ninputs = 2,
    noutputs = 2,

    inputs = {
        {
            name = "input1",
            description = "This probably the left audio input signal."
        },
        {
            name = "input2",
            description = "This probably the right audio input signal."
        },
    },

    outputs = {
        {
            name = "out_left",
            description = "Stereo left output for phaser."
        },
        {
            name = "out_right",
            description = "Stereo right output for phaser."
        },
    }

}

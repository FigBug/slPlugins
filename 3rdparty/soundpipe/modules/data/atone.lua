sptbl["atone"] = {

	files = {
	    module = "atone.c",
	    header = "atone.h",
	    example = "ex_atone.c",
	},

	func = {
	    create = "sp_atone_create",
	    destroy = "sp_atone_destroy",
	    init = "sp_atone_init",
	    compute = "sp_atone_compute",
	},

	params = {
	    optional = {
	        {
	            name = "hp",
	            type = "SPFLOAT",
	            description = "The response curve's half power point (cutoff frequency).",
	            default = "1000"
	        },
	    },
	},

	modtype = "module",

	description = [[First-order recursive highpass filter

	This is the complement to the tone module.]],

	ninputs = 1,
	noutputs = 1,

	inputs = {
	    {
	        name = "in",
	        description = "Audio signal input."
	    },
	},

	outputs = {
	    {
	        name = "out",
	        description = "Audio signal output."
	    },
	}

}

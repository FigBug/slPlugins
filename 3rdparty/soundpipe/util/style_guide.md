Soundpipe Style Guide
=====================

If you would like to contribute to Soundpipe, the code you make should conform
to these guidelines. This is a work in progress.

# C Code Conventions

The C code in Soundpipe follows most of the [C style 
guidelines](http://suckless.org/coding_style) found on the
suckless.org website with few exceptions. If you're ever in doubt about 
something not specified here, use the suckless style guide, or the [Linux 
Kernel style guide](https://www.kernel.org/doc/Documentation/CodingStyle).

Please use K&R style braces, as shown below:

```
int the_important_function()
{
	...
	if (something_is_true) {
		...
	}
	...
}
```

If you are creating a new module, the expectation is that all code for that
module resides in a single c and header file.

When creating modules, please take care not to pollute the
namespace. If a function for a particular module is not in the typical 
lifecycle (create, init, destroy, compute) and doesn't need to be
directly accessed, it probably should be a static function. 

Soundpipe code should have no more than 80 characters per line. 

# Commenting

Comments in Soundpipe are rather sparse, unless they make up the header of a 
module file. 
There's a reason for this. DSP code in C is hard to read even with comments. 
The algorithm
used in your code should be clear after studying the code. If you are making a 
module and 
would like to explain what it does in depth, you should document it properly 
using Soundpipe's
documentation format.

If you do need to comment, use *only* /\* \*/ commenting conventions.

The Linux Kernel convention for commenting is preferred for multiline - block 
comments.

If you are doing something necessarily clever, make a comment about it.

If you are using DSP code or algorithms from someone else, keep it in the 
header of the .c file. 
There are several ported Csound opcodes you can use as reference (metro is a 
good example).


# Variable Naming

In C, a good variable name has the right balance of terseness and verboseness 
for maximum clarity and grokking. As an example, abbreviating "frequency" 
to "freq" is acceptable, but abbreviating "filter_cutoff" to "fc" is too ambiguous. 

Variables should be one word, but if they must use more than one word, 
use under\_score\_variables and not camelCase.

Soundpipe has a lot of accepted abbreviations for certain variables. These are 
examples of okay names to use for module parameters:

- freq for frequency (do not use cps)
- amp for amplitude
- mod for modulation frequency
- car for carrier frequency
- atk for attack
- dec for decay
- rel for release
- dist for distortion
- filt for filter
- bw for bandwidth
- sr for samplerate
- res for resonance
- prev for previous

If you are porting Csound code, there is often a tendency to put a, k, or i in 
front
of a variable to indicate audio, control, or init-rate variables. Ditch the a's 
and k's...
i's can be allowed if they are init-time variables.

# Warnings and Errors

If you notice any compiler warnings after adding your code, please 
investigate them. If you're unsure of a 
warning and would like some help, please feel free to ask when you
make your pull request. 

When creating an example to highlight a module, there should be
no errors in Valgrind.

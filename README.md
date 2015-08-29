# SI Plugins

This repository contains the SI plugin series, which I am writing mainly for my own
edification. The name SI is short for "Sherman Innovations". Note that the bulk of this repository
was copied from https://github.com/DISTRHO/plugin-examples.git - I cleared out the examples so I
could build my own plugins.

# License

The DPF (Distrho plugin framework) can be found here: http://github.com/DISTRHO. For its license see
LICENSE_DFP. The plugins themselves are under GPL2, and you can see that in LICENSE_SIPLUGINS.

# Building

The system uses a fairly straight forward set of make files, but you'll need to get the dependencies sorted
first. If you're trying to build on Windows, I recommend using MSYS2, and if you can't find any packages
look at https://github.com/guysherman/MINGW-packages.git

## Dependencies
You'll need the following installed such that you can build against them (ie you need headers and libs):
	*	jack
	*	liblo	(if you want OSC in your plugins)
	*	LV2 (lv2-dev)
	*	LADSPA (ladspa-sdk)

## Compiling

From the root folder:

	$ make

Its that simple, look in the `bin` folder and you'll have the plugin(s) built. You'll need to copy the plugins
to the right place to use them, I found that /usr/local/lib/ladspa and /usr/local/lib/lv2 worked respectively for
ladspa and lv2.

# Other notes

Feel free to learn from what I have put here, and I certainly welcome any constructive comments.

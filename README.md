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

Make sure you get the submodules as well

```
$ git submodule update --init
```

## Dependencies
You'll need the following installed such that you can build against them (ie you need headers and libs):
*	jack
*	liblo	(if you want OSC in your plugins)
*	LV2 (lv2-dev)
*	LADSPA (ladspa-sdk)
*	libsamplerate (http://www.mega-nerd.com/SRC/)

## Compiling

From the root folder:

	$ make

Its that simple, look in the `bin` folder and you'll have the plugin(s) built. You'll need to copy the plugins
to the right place to use them, I found that /usr/local/lib/ladspa and /usr/local/lib/lv2 worked respectively for
ladspa and lv2.


# Plugins

## SI-D1 - SI Distortion #1
This is a really basic ditortion, based off the idea of how a simple analogue overdrive works: a diode network is used
such that when the voltage gets high enough, some current flows backgwards through the diode to ground. This way, there is
a hard limit on the voltage across the output. So I did the same with the plugin - it's really rough overdrive because it
is very hard clipping, literally a set of conditional statements clamping the output.

There are three knobs on the plugin: Gain, Bias and Range.
*	Gain controls the pre-amplification
*	Bias controls where the centre of the clipping range is, but it is not a DC bias applied to the signal,
	rather it is a bias applied to the clipping thresholds.
*	Range sets the distance between the clipping thresholds. So, with a bias of 0 and a range of 1, signals will clip at +/- 0.5.

## SI-D2 - SI Distorion #2
This is a slightly more advanced distortion - tanh(ax) where `a` is a coefficient that can range from 0.5 to 2, and x is the input sample. This gives a smooth(ish) edge to the clipping, which the user can control with the 'Slope' parameter.

There are three knobs:
*	Gain - pre-amplification
*	Slope - the coefficient `a` in tanh(ax)
*	Level - essentially an attenuator (in case it gets too loud)




# Other notes

Feel free to learn from what I have put here, and I certainly welcome any constructive comments.

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

## Cross-compiling for Windows

This can be done using the mingw-w64 toolchain. On Ubuntu I installed it with
```
sudo apt install mingw-w64
```

You will need to cross-compile libsamplerate (http://www.mega-nerd.com/SRC/) first. This was easily done with :

```
./configure --build=x86_64-linux host=x86_64-w64-mingw32 --enable-shared=no --enable-static=yes
make
make install
```

NB: because I was using an Ubuntu environment only used for cross-compilation, I was happy to install
my cross-compiled libs into /usr/local, but you may want to specify a different --prefix when you configure it. If you do so, you will need to edit the following two files in this repository:

```
<repo root>/Makefile.mk
<repo root>/dpf/dgl/Makefile.mk
```

You will find near the end of these two files, some extra flags for cross-compilation, and you'll need
to add your prefix root to the include and link flags respectively.

One step that will have to be done separately is generating the lv2 ttl files. This will either need to be done with the linux version of the tool (created via a standard linux build), or with the windows exe, run from the Windows command prompt. If running on Windows do the following from the source root (assuming a cross-compile build):

```
dpf\utils\generate-ttl.bat
```


# Plugins

## SI-D1 - SI Distortion #1
This is a really basic distortion, based off the idea of how a simple analogue overdrive works: a diode network is used
such that when the voltage gets high enough, some current flows backwards through the diode to ground. This way, there is
a hard limit on the voltage across the output. So I did the same with the plugin - it's really rough overdrive because it
is very hard clipping, literally a set of conditional statements clamping the output.

There are three knobs on the plugin: Gain, Bias and Range.
*	Gain controls the pre-amplification
*	Bias controls where the centre of the clipping range is, but it is not a DC bias applied to the signal,
	rather it is a bias applied to the clipping thresholds.
*	Range sets the distance between the clipping thresholds. So, with a bias of 0 and a range of 1, signals will clip at +/- 0.5.

## SI-D2 - SI Distortion #2
This is a slightly more advanced distortion - tanh(ax) where `a` is a coefficient that can range from 0.5 to 2, and x is the input sample. This gives a smooth(ish) edge to the clipping, which the user can control with the 'Slope' parameter.

There are three knobs:
*	Gain - pre-amplification
*	Slope - the coefficient `a` in tanh(ax)
*	Level - essentially an attenuator (in case it gets too loud)


## SI-L1 - SI Low-pass Filter #1
This is a really simple, first-order low-pass filter of the form:

```y[i] = B * x[i] + (1-B) * y[i-1]```

Where x is the stream of inputs, and y is the stream of output values, and `B` is calculated from the 'Cutoff' parameter through the formula:

```B = 1 - e^(((-2*PI)*Cutoff)/SampleRate)```

## SI-L2 - SI Low-pass Filter #2
This is a Butterworth filter of the form:

```y[i] = b0 * x[i] + b1 * x[i-1] + b2 * x[i-2] + a1 * y[i-1] + a2 * [i-2]```

Where `x` is the stream of inputs, and `y` is the stream of output values, and the coefficients are calculated as follows:

```
# these first three are intermediate values
float ff = cutoff/sample;
const double ita = 1.0 / tan(M_PI*ff);
const double q = sqrt(2.0);

b0 = 1.0 / (1.0 + q*ita + ita*ita);
b1 = 2*b0;
b2 = b0;

a1 = 2.0 * (ita*ita - 1.0) * b0;
a2 = -1*(1.0 - q*ita + ita*ita) * b0;
```


## SI-H1 - SI High-pass Filter #1
This is a really simple, first-order high-pass filter of the form:

```y[i] = B * (y[i-1] + x[i] - x[i-1])```

Where x is the stream of inputs, and y is the stream of output values and `B` is calculated from the 'Cutoff' parameter through the formula:

```B = e^(((-2*PI)*Cutoff)/SampleRate)```

## SI-H2 - SI High-pass Filter #2

This is also a 2nd order Butterworth filter, but by applying a few extra steps in calculating the coefficients, we get a High-pass filter instead. Apparently.

Having calculated the coefficients as in SI-L2, we do the following:

```
b0 = b0 * ita * ita;
b1 = -b1 * ita * ita;
b2 = b2 * ita * ita;
```




# Other notes

Feel free to learn from what I have put here, and I certainly welcome any constructive comments.

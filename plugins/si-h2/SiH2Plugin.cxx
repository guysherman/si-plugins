/*
  Copyright (C) 2015 Guy Sherman, Shermann Innovations Limited

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

// C++ Standard Headers


// C Standard Headers
#include <cmath>
#include <cassert>
#include <stdio.h>

// Boost Headers


// 3rd Party Headers
#include <samplerate.h>

// GTK Headers


#include "SiH2Plugin.hxx"

namespace sherman
{
	SiH2Plugin::SiH2Plugin()
		: Plugin(paramCount, 1, 0),
		slope(-6.0f),
		cutoff(100.0f),
		lastBufferLastSampleIn(0.0f),
		lastBufferLastSampleOut(0.0f),
		x1(0.0f),
		x2(0.0f),
		y1(0.0f),
		y2(0.0f),
		a1(0.0f),
		a2(0.0f),
		b0(0.0f),
		b1(0.0f),
		b2(0.0f),
		active(1.0f)
	{
		loadProgram(0);
		deactivate();
	}

	SiH2Plugin::~SiH2Plugin()
	{
	}

	void  SiH2Plugin::loadProgram(uint32_t index)
	{
		if (index != 0)
		{
			return;
		}

		slope = -6.0f;
		cutoff = 100.0f;


		activate();
	}

	void SiH2Plugin::activate()
	{
		design();
		active = 1.0f;
	}

	void SiH2Plugin::design()
	{
		float rate = (float)getSampleRate();
		float ff = cutoff/rate;
		const double ita = 1.0 / tan(M_PI*ff);
		const double q = sqrt(2.0);

		b0 = (1.0 / (1.0 + q*ita + ita*ita));
		b1 = 2*b0;
		b2 = b0;

		a1 = 2.0 * (ita*ita - 1.0) * b0;
		a2 = -1*(1.0 - q*ita + ita*ita) * b0;

		b0 = b0 * ita * ita;
		b1 = -b1 * ita * ita;
		b2 = b2 * ita * ita;
	}

	void SiH2Plugin::deactivate()
	{
		active = 0.0f;
	}

	void SiH2Plugin::initParameter(uint32_t index, Parameter& parameter)
	{
		switch (index)
		{
			// case paramSlope:
			// 	setupParamSlope(parameter);
			// 	break;
			case paramCutoff:
				setupParamCutoff(parameter);
				break;

		}
	}

	void SiH2Plugin::setupParamSlope(Parameter& parameter)
	{
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Slope";
		parameter.symbol = "slope";
		parameter.unit = "dB";
		parameter.ranges.def = -6.0f;
		parameter.ranges.min = -48.0f;
		parameter.ranges.max = 0.0f;
	}

	void SiH2Plugin::setupParamCutoff(Parameter& parameter)
	{
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Cutoff";
		parameter.symbol = "cutoff";
		parameter.unit = "Hz";
		parameter.ranges.def = 100.0f;
		parameter.ranges.min = 10.0f;
		parameter.ranges.max = 1000.0f;
	}

	void SiH2Plugin::initProgramName(uint32_t index, String& programName)
	{
		if (index != 0)
		{
			return;
		}

		programName = "Default";
	}

	// -------------------------------------------------------------------
	// Internal data

	float SiH2Plugin::getParameterValue(uint32_t index) const
	{
		// [db] = 20 * log ([coeff])
		// the inverse is  coeff = 10^(0.05 * [db])

		switch(index)
		{
			// case paramSlope:
			// 	return getSlope();
			case paramCutoff:
				return getCutoff();

		}

		return 0;
	}

	float SiH2Plugin::getSlope() const
	{
		return slope;
	}

	float SiH2Plugin::getCutoff() const
	{
		return cutoff;
	}

	void  SiH2Plugin::setParameterValue(uint32_t index, float value)
	{
		switch (index)
		{
			// case paramSlope:
			// 	setSlope(value);
			// 	break;
			case paramCutoff:
				setCutoff(value);
				break;

		}
	}

	void SiH2Plugin::setSlope(float slope)
	{
		this->slope = slope;
	}

	void SiH2Plugin::setCutoff(float cutoff)
	{
		this->cutoff = cutoff;
		design();
	}



	void SiH2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
	{
		outputs[0][0] = (b0 * inputs[0][0]) + (b1 * x1) + (b2 * x2) + (a1 * y1) + (a2 * y2);
		outputs[0][1] = (b0 * inputs[0][1]) + (b1 * inputs[0][0]) + (b2 * x1) + (a1 * outputs[0][0]) + (a2 * y1);
		for (uint32_t i = 2; i < frames; ++i)
		{
			outputs[0][i] = (b0 * inputs[0][i]) + (b1 * inputs[0][i-1]) + (b2 * inputs[0][i-2]) + (a1 * outputs[0][i-1]) + (a2 * outputs[0][i-2]);
		}

		x1 = inputs[0][frames-1];
		x2 = inputs[0][frames-2];
		y1 = outputs[0][frames-1];
		y2 = outputs[0][frames-2];
	}


}

START_NAMESPACE_DISTRHO

Plugin* createPlugin()
{
	return new sherman::SiH2Plugin();
}

END_NAMESPACE_DISTRHO

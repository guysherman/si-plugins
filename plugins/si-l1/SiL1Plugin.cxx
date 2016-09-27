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


#include "SiL1Plugin.hxx"

namespace sherman
{
	SiL1Plugin::SiL1Plugin()
		: Plugin(paramCount, 1, 0),
		slope(0.25f),
		cutoff(8000.0f),
		lastBufferLastSample(0.0f),
		beta(0.25f),
		active(1.0f)
	{
		loadProgram(0);
		deactivate();
	}

	SiL1Plugin::~SiL1Plugin()
	{
	}

	void  SiL1Plugin::loadProgram(uint32_t index)
	{
		if (index != 0)
		{
			return;
		}

		slope = -3.0f;
		cutoff = 8000.0f;

		activate();
	}

	void SiL1Plugin::activate()
	{
		active = 1.0f;
	}

	void SiL1Plugin::deactivate()
	{
		active = 0.0f;
	}

	void SiL1Plugin::initParameter(uint32_t index, Parameter& parameter)
	{
		switch (index)
		{
			case paramSlope:
				setupParamSlope(parameter);
				break;
			case paramCutoff:
				setupParamCutoff(parameter);
				break;

		}
	}

	void SiL1Plugin::setupParamSlope(Parameter& parameter)
	{
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Slope";
		parameter.symbol = "slope";
		parameter.unit = "dB";
		parameter.ranges.def = 0.25f;
		parameter.ranges.min = 0.0f;
		parameter.ranges.max = 1.0f;
	}

	void SiL1Plugin::setupParamCutoff(Parameter& parameter)
	{
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Cutoff";
		parameter.symbol = "cutoff";
		parameter.unit = "Hz";
		parameter.ranges.def = 8000.0f;
		parameter.ranges.min = 630.0f;
		parameter.ranges.max = 20000.0f;
	}

	void SiL1Plugin::initProgramName(uint32_t index, String& programName)
	{
		if (index != 0)
		{
			return;
		}

		programName = "Default";
	}

	// -------------------------------------------------------------------
	// Internal data

	float SiL1Plugin::getParameterValue(uint32_t index) const
	{
		// [db] = 20 * log ([coeff])
		// the inverse is  coeff = 10^(0.05 * [db])

		switch(index)
		{
			case paramSlope:
				return getSlope();
			case paramCutoff:
				return getCutoff();

		}

		return 0;
	}

	float SiL1Plugin::getSlope() const
	{
		return slope;
	}

	float SiL1Plugin::getCutoff() const
	{
		return cutoff;
	}

	void  SiL1Plugin::setParameterValue(uint32_t index, float value)
	{
		switch (index)
		{
			case paramSlope:
				setSlope(value);
				break;
			case paramCutoff:
				setCutoff(value);
				break;

		}
	}

	void SiL1Plugin::setSlope(float slope)
	{
		this->slope = slope;
	}

	void SiL1Plugin::setCutoff(float cutoff)
	{
		this->cutoff = cutoff;
	}



	void SiL1Plugin::run(const float** inputs, float** outputs, uint32_t frames)
	{
		beta = slope;
		outputs[0][0] = beta * inputs[0][0] + (1-beta) * lastBufferLastSample;
		for (uint32_t i = 1; i < frames; ++i)
		{
			outputs[0][i] = beta * inputs[0][i] + (1-beta) * outputs[0][i-1];
		}

		lastBufferLastSample = outputs[0][frames-1];
	}


}

START_NAMESPACE_DISTRHO

Plugin* createPlugin()
{
	return new sherman::SiL1Plugin();
}

END_NAMESPACE_DISTRHO

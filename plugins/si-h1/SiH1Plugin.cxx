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


#include "SiH1Plugin.hxx"

namespace sherman
{
	SiH1Plugin::SiH1Plugin()
		: Plugin(paramCount, 1, 0),
		slope(-6.0f),
		cutoff(100.0f),
		lastBufferLastSampleIn(0.0f),
		lastBufferLastSampleOut(0.0f),
		beta(0.0f),
		active(1.0f)
	{
		loadProgram(0);
		deactivate();
	}

	SiH1Plugin::~SiH1Plugin()
	{
	}

	void  SiH1Plugin::loadProgram(uint32_t index)
	{
		if (index != 0)
		{
			return;
		}

		slope = -6.0f;
		cutoff = 100.0f;


		activate();
	}

	void SiH1Plugin::activate()
	{
		float rate = (float)getSampleRate();
		//this->beta = 1 - pow(M_E, (((-2*M_PI)*cutoff)/rate));
		//printf("Rate: %f.4; Beta: %f.6\n", rate, this->beta);
		//this->beta = (cutoff - 10) / 990;
		this->beta = pow(M_E, (((-2*M_PI)*cutoff)/rate));
		active = 1.0f;
	}

	void SiH1Plugin::deactivate()
	{
		active = 0.0f;
	}

	void SiH1Plugin::initParameter(uint32_t index, Parameter& parameter)
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

	void SiH1Plugin::setupParamSlope(Parameter& parameter)
	{
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Slope";
		parameter.symbol = "slope";
		parameter.unit = "dB";
		parameter.ranges.def = -6.0f;
		parameter.ranges.min = -48.0f;
		parameter.ranges.max = 0.0f;
	}

	void SiH1Plugin::setupParamCutoff(Parameter& parameter)
	{
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Cutoff";
		parameter.symbol = "cutoff";
		parameter.unit = "Hz";
		parameter.ranges.def = 100.0f;
		parameter.ranges.min = 10.0f;
		parameter.ranges.max = 1000.0f;
	}

	void SiH1Plugin::initProgramName(uint32_t index, String& programName)
	{
		if (index != 0)
		{
			return;
		}

		programName = "Default";
	}

	// -------------------------------------------------------------------
	// Internal data

	float SiH1Plugin::getParameterValue(uint32_t index) const
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

	float SiH1Plugin::getSlope() const
	{
		return slope;
	}

	float SiH1Plugin::getCutoff() const
	{
		return cutoff;
	}

	void  SiH1Plugin::setParameterValue(uint32_t index, float value)
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

	void SiH1Plugin::setSlope(float slope)
	{
		this->slope = slope;
	}

	void SiH1Plugin::setCutoff(float cutoff)
	{
		float rate = (float)getSampleRate();
		this->cutoff = cutoff;
		//this->beta = 1 - pow(M_E, (((-2*M_PI)*cutoff)/rate));
		//this->beta = (cutoff - 10) / 990;
		this->beta = pow(M_E, (((-2*M_PI)*cutoff)/rate));
		//printf("Rate: %f.4; Beta: %f.6\n", rate, this->beta);
	}



	void SiH1Plugin::run(const float** inputs, float** outputs, uint32_t frames)
	{
		outputs[0][0] = beta * (lastBufferLastSampleOut + inputs[0][0] - lastBufferLastSampleIn);
		for (uint32_t i = 1; i < frames; ++i)
		{
			outputs[0][i] = beta * (outputs[0][i-1] + inputs[0][i] - inputs[0][i-1]);
		}

		lastBufferLastSampleOut = outputs[0][frames-1];
		lastBufferLastSampleIn = inputs[0][frames-1];
	}


}

START_NAMESPACE_DISTRHO

Plugin* createPlugin()
{
	return new sherman::SiH1Plugin();
}

END_NAMESPACE_DISTRHO

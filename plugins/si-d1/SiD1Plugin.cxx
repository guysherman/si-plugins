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

// Boost Headers


// 3rd Party Headers


// GTK Headers


#include "SiD1Plugin.hxx"

namespace sherman
{
    SiD1Plugin::SiD1Plugin()
        : Plugin(paramCount, 1, 0),
        gainDb(18.0f), gainCoeff(7.943f),
        bias(0.3f),
        active(1.0f), distance(1.2f), negClip(0.0f), posClip(0.0f)
    {
        loadProgram(0);

        deactivate();
    }

    void  SiD1Plugin::loadProgram(uint32_t index)
    {
        if (index != 0)
        {
            return;
        }

        gainDb = 18.0f;
        bias = 0.3f;
        distance = 1.2f;

        activate();
    }

    void SiD1Plugin::activate()
    {
        gainCoeff = pow(10, 0.05 * gainDb);
        recalculateClips();
        active = 1.0f;
    }

    void SiD1Plugin::recalculateClips()
    {
        posClip = distance / 2.0f + bias;
        negClip = -1.0f * (distance / 2.0f) + bias;
    }

    void SiD1Plugin::deactivate()
    {
        active = 0.0f;
    }

    void SiD1Plugin::initParameter(uint32_t index, Parameter& parameter)
    {
        switch (index)
        {
            case paramGain:
                setupParamGain(parameter);
                break;
            case paramBias:
                setupParamBias(parameter);
                break;
            case paramDistance:
                setupParamDistance(parameter);
                break;

        }
    }

    void SiD1Plugin::setupParamGain(Parameter& parameter)
    {
        parameter.hints = kParameterIsAutomable;
        parameter.name = "Gain";
        parameter.symbol = "gain";
        parameter.unit = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 48.0f;
    }

    void SiD1Plugin::setupParamBias(Parameter& parameter)
    {
        parameter.hints = kParameterIsAutomable;
        parameter.name = "Bias";
        parameter.symbol = "bias";
        parameter.unit = "";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -1.0f;
        parameter.ranges.max = 1.0f;
    }

    void SiD1Plugin::setupParamDistance(Parameter& parameter)
    {
        parameter.hints = kParameterIsAutomable;
        parameter.name = "Distance";
        parameter.symbol = "distance";
        parameter.unit = "";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 2.0f;
    }

    void SiD1Plugin::initProgramName(uint32_t index, String& programName)
    {
        if (index != 0)
        {
            return;
        }

        programName = "Default";
    }

    // -------------------------------------------------------------------
    // Internal data

    float SiD1Plugin::getParameterValue(uint32_t index) const
    {
        // [db] = 20 * log ([coeff])
        // the inverse is  coeff = 10^(0.05 * [db])

        switch(index)
        {
            case paramGain:
                return getGain();
            case paramBias:
                return getBias();
            case paramDistance:
                return getDistance();

        }

        return 0;
    }

    float SiD1Plugin::getGain() const
    {
        return gainDb;
    }

    float SiD1Plugin::getBias() const
    {
        return bias;
    }

    float SiD1Plugin::getDistance() const
    {
        return distance;
    }

    void  SiD1Plugin::setParameterValue(uint32_t index, float value)
    {
        switch (index)
        {
            case paramGain:
                setGain(value);
                break;
            case paramBias:
                setBias(value);
                break;
            case paramDistance:
                setDistance(value);
                break;

        }
    }

    void SiD1Plugin::setGain(float gain)
    {
        gainDb = gain;
        gainCoeff = pow(10, 0.05 * gainDb);
    }

    void SiD1Plugin::setBias(float bias)
    {
        this->bias = bias;
        recalculateClips();
    }

    void SiD1Plugin::setDistance(float distance)
    {
        this->distance = distance;
        recalculateClips();
    }



    void SiD1Plugin::run(const float** inputs, float** outputs, uint32_t frames)
    {
        const float* in1  = inputs[0];
        float*       out1 = outputs[0];

        for (uint32_t i = 0; i < frames; ++i)
        {
            float amplified1 = (in1[i]) * gainCoeff;

            float clipped1 = (amplified1 < negClip) ? negClip : ((amplified1 > posClip) ? posClip : amplified1);

            out1[i] = clipped1 * active;
        }
    }


}

START_NAMESPACE_DISTRHO

Plugin* createPlugin()
{
    return new sherman::SiD1Plugin();
}

END_NAMESPACE_DISTRHO

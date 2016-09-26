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


#include "SiD2Plugin.hxx"

namespace sherman
{
    SiD2Plugin::SiD2Plugin()
        : Plugin(paramCount, 1, 0),
        gainDb(18.0f), gainCoeff(7.943f),
        slope(0.3f),
        active(1.0f), level(1.2f), upConverter(NULL), downConverter(NULL), upConverterError(0), downConverterError(0)
    {
        memset(&resampled_1, 0x00, 4096);
        memset(&resampled_2, 0x00, 4096);

        upConverter = src_new(SRC_SINC_BEST_QUALITY, 1, &upConverterError);
        downConverter = src_new(SRC_SINC_BEST_QUALITY, 1, &downConverterError);

        loadProgram(0);

        deactivate();
    }

    SiD2Plugin::~SiD2Plugin()
    {
        src_delete(upConverter);
        src_delete(downConverter);
    }

    void  SiD2Plugin::loadProgram(uint32_t index)
    {
        if (index != 0)
        {
            return;
        }

        gainDb = 30.0f;
        slope = 1.0f;
        level = 1.0f;

        activate();
    }

    void SiD2Plugin::activate()
    {
        gainCoeff = pow(10, 0.05 * gainDb);
        src_reset(upConverter);
        src_reset(downConverter);
        active = 1.0f;
    }

    void SiD2Plugin::deactivate()
    {
        active = 0.0f;
    }

    void SiD2Plugin::initParameter(uint32_t index, Parameter& parameter)
    {
        switch (index)
        {
            case paramGain:
                setupParamGain(parameter);
                break;
            case paramSlope:
                setupParamSlope(parameter);
                break;
            case paramLevel:
                setupParamLevel(parameter);
                break;

        }
    }

    void SiD2Plugin::setupParamGain(Parameter& parameter)
    {
        parameter.hints = kParameterIsAutomable;
        parameter.name = "Gain";
        parameter.symbol = "gain";
        parameter.unit = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 48.0f;
    }

    void SiD2Plugin::setupParamSlope(Parameter& parameter)
    {
        parameter.hints = kParameterIsAutomable;
        parameter.name = "Slope";
        parameter.symbol = "slope";
        parameter.unit = "";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.5f;
        parameter.ranges.max = 2.0f;
    }

    void SiD2Plugin::setupParamLevel(Parameter& parameter)
    {
        parameter.hints = kParameterIsAutomable;
        parameter.name = "Level";
        parameter.symbol = "level";
        parameter.unit = "";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
    }

    void SiD2Plugin::initProgramName(uint32_t index, String& programName)
    {
        if (index != 0)
        {
            return;
        }

        programName = "Default";
    }

    // -------------------------------------------------------------------
    // Internal data

    float SiD2Plugin::getParameterValue(uint32_t index) const
    {
        // [db] = 20 * log ([coeff])
        // the inverse is  coeff = 10^(0.05 * [db])

        switch(index)
        {
            case paramGain:
                return getGain();
            case paramSlope:
                return getSlope();
            case paramLevel:
                return getLevel();

        }

        return 0;
    }

    float SiD2Plugin::getGain() const
    {
        return gainDb;
    }

    float SiD2Plugin::getSlope() const
    {
        return slope;
    }

    float SiD2Plugin::getLevel() const
    {
        return level;
    }

    void  SiD2Plugin::setParameterValue(uint32_t index, float value)
    {
        switch (index)
        {
            case paramGain:
                setGain(value);
                break;
            case paramSlope:
                setSlope(value);
                break;
            case paramLevel:
                setLevel(value);
                break;

        }
    }

    void SiD2Plugin::setGain(float gain)
    {
        gainDb = gain;
        gainCoeff = pow(10, 0.05 * gainDb);
    }

    void SiD2Plugin::setSlope(float slope)
    {
        this->slope = slope;
    }

    void SiD2Plugin::setLevel(float level)
    {
        this->level = level;
    }



    void SiD2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
    {
        const float* in1  = &inputs[0][0];
        float*       out1 = &outputs[0][0];

        SRC_DATA args_in;
        args_in.data_in = (float*)in1;        // I feel a bit dirty right now
        args_in.data_out = &resampled_1[0];
        args_in.input_frames = frames;
        args_in.output_frames = frames * 2;
        args_in.input_frames_used = 0;
        args_in.output_frames_gen = 0;
        args_in.end_of_input = 0;
        args_in.src_ratio = 2;

        src_process(upConverter, &args_in);



        for (uint32_t i = 0; i < args_in.output_frames_gen; ++i)
        //for (uint32_t i = 0; i < frames; ++i)
        {
            float amplified1 = (resampled_1[i]) * gainCoeff;

            float result = this->level *tanh(this->slope * amplified1);
            //float result = tanh(4*in1[i]);

            resampled_2[i] = result * active;
            //out1[i] = result * active;
        }

        SRC_DATA args_out;
        args_out.data_in = &resampled_2[0];;
        args_out.data_out = out1;
        args_out.input_frames = args_in.output_frames_gen;
        args_out.output_frames = args_in.input_frames_used;
        args_out.input_frames_used = 0;
        args_out.output_frames_gen = 0;
        args_out.end_of_input = 0;
        args_out.src_ratio = 0.5;

        src_process(downConverter, &args_out);
        //printf("Num frames: %d, Num gen frames: %d, frames * 2: %d", args_in.input_frames_used, args_out.output_frames_gen, args_in.input_frames_used * 0.5);
        //assert(args_in.output_frames_gen == frames * 2);

    }


}

START_NAMESPACE_DISTRHO

Plugin* createPlugin()
{
    return new sherman::SiD2Plugin();
}

END_NAMESPACE_DISTRHO

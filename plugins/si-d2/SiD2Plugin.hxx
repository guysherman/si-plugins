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

#ifndef __SiD2Plugin_HXX__
#define __SiD2Plugin_HXX__

// C++ Standard Headers


// C Standard Headers


// Boost Headers


// 3rd Party Headers
#include <samplerate.h>
#include "DistrhoPlugin.hpp"

// GTK Headers

namespace sherman
{
	class SiD2Plugin : public Plugin
	{
	public:
		enum Parameters
		{
			paramGain = 0,
			paramSlope,
			paramLevel,
			paramCount
		};

		SiD2Plugin();
		virtual ~SiD2Plugin();

	protected:
		const char* getLabel() const noexcept override
    {
        return "Distortion-2";
    }

    const char* getMaker() const noexcept override
    {
        return "SHERMAN";
    }

    const char* getLicense() const noexcept override
    {
        return "LGPL";
    }

    uint32_t getVersion() const noexcept override
    {
        return 0x1000;
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('S', 'I', 'D', '2');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;
    void initProgramName(uint32_t index, String& programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index) override;

    // -------------------------------------------------------------------
    // Process

    void activate() override;
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

	private:
		void setupParamGain(Parameter& parameter);
		void setupParamSlope(Parameter& parameter);
		void setupParamLevel(Parameter& parameter);

		void setGain(float gain);
		float getGain() const;

		void setSlope(float slope);
		float getSlope() const;

		void setLevel(float level);
		float getLevel() const;


		float gainDb;
		float gainCoeff;
		float slope;
		float level;
		float active;

		float resampled_1[4096];
		float resampled_2[4096];

		SRC_STATE* upConverter;
		SRC_STATE* downConverter;
		int upConverterError;
		int downConverterError;

		DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SiD2Plugin)
	};
}




#endif // __SiD2Plugin_HXX__

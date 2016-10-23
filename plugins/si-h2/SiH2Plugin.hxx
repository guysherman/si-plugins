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

#ifndef __SiH2Plugin_HXX__
#define __SiH2Plugin_HXX__

// C++ Standard Headers


// C Standard Headers


// Boost Headers


// 3rd Party Headers
#include <samplerate.h>
#include "DistrhoPlugin.hpp"

// GTK Headers

namespace sherman
{
	class SiH2Plugin : public Plugin
	{
	public:
		enum Parameters
		{
			paramSlope = 0,
			paramCutoff,
			paramCount
		};

		SiH2Plugin();
		virtual ~SiH2Plugin();

	protected:
		const char* getLabel() const noexcept override
	{
		return "HighPass-2";
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
		return d_cconst('S', 'I', 'H', '2');
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
		void setupParamSlope(Parameter& parameter);
		void setupParamCutoff(Parameter& parameter);

		void setSlope(float slope);
		float getSlope() const;

		void setCutoff(float cutoff);
		float getCutoff() const;

		void design();

		float slope;
		float cutoff;
		float lastBufferLastSampleIn;
		float lastBufferLastSampleOut;
		float x1, x2, y1, y2;
		float a1, a2, b0, b1, b2;
		float active;


		DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SiH2Plugin)
	};
}




#endif // __SiH2Plugin_HXX__

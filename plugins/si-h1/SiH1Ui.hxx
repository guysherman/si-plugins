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

#ifndef __SiH1UI_HXX__
#define __SiH1UI_HXX__


// C++ Standard Headers


// C Standard Headers


// Boost Headers


// 3rd Party Headers


// GTK Headers


#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"

#include "SiH1Assets.hpp"
//#include "DistrhoArtwork3BandEQ.hpp"

namespace sherman
{
	class SiH1Ui : public UI, public ImageKnob::Callback
	{
	public:
		SiH1Ui();

	protected:
		// -------------------------------------------------------------------
		// DSP Callbacks

		void parameterChanged(uint32_t index, float value) override;
		void programLoaded(uint32_t index) override;

		// -------------------------------------------------------------------
		// Widget Callbacks

		void imageKnobDragStarted(ImageKnob* knob) override;
		void imageKnobDragFinished(ImageKnob* knob) override;
		void imageKnobValueChanged(ImageKnob* knob, float value) override;

		void onDisplay() override;

	private:
		Image backgroundImage;

		//ScopedPointer<ImageKnob> slopeKnob;
		ScopedPointer<ImageKnob> cutoffKnob;


		DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SiH1Ui);

	};
}


#endif // __SiH1UI_HXX__

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


// Boost Headers


// 3rd Party Headers


// GTK Headers

#include "SiL2Plugin.hxx"
#include "SiL2Ui.hxx"

namespace Art = SiL2Assets;
//namespace Art = DistrhoArtwork3BandEQ;

namespace sherman
{

SiL2Ui::SiL2Ui()
	: UI(Art::backgroundWidth, Art::backgroundHeight),
	backgroundImage(Art::backgroundData, Art::backgroundWidth, Art::backgroundHeight, GL_BGRA)
{
	Image knobImage(Art::knobData, Art::knobWidth, Art::knobHeight);
	// slopeKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	// slopeKnob->setId(SiL2Plugin::paramSlope);
	// slopeKnob->setAbsolutePos(15, 15);
	// slopeKnob->setRange(-48.0f, 0.0f);
	// slopeKnob->setDefault(-6.0f);
	// slopeKnob->setRotationAngle(240);
	// slopeKnob->setCallback(this);

	cutoffKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	cutoffKnob->setId(SiL2Plugin::paramCutoff);
	cutoffKnob->setAbsolutePos(215, 15);
	cutoffKnob->setRange(630.0f, 20000.0f);
	cutoffKnob->setDefault(8000.0f);
	cutoffKnob->setRotationAngle(240);
	cutoffKnob->setCallback(this);

	programLoaded(0);
}


	// -------------------------------------------------------------------
	// DSP Callbacks

	void SiL2Ui::parameterChanged(uint32_t index, float value)
	{
		switch(index)
		{
			//case SiL2Plugin::paramSlope:
			//slopeKnob->setValue(value);
			//break;
			case SiL2Plugin::paramCutoff:
			cutoffKnob->setValue(value);
			break;
		}
	}


	void SiL2Ui::programLoaded(uint32_t index)
	{
		if (index != 0)
		{
			return;
		}

		//slopeKnob->setValue(-3.0f);
	}

	// -------------------------------------------------------------------
	// Widget Callbacks

	void SiL2Ui::imageKnobDragStarted(ImageKnob* knob)
	{
		editParameter(knob->getId(), true);
	}

	void SiL2Ui::imageKnobDragFinished(ImageKnob* knob)
	{
		editParameter(knob->getId(), false);
	}

	void SiL2Ui::imageKnobValueChanged(ImageKnob* knob, float value)
	{
		setParameterValue(knob->getId(), value);
	}

	void SiL2Ui::onDisplay()
	{
		backgroundImage.draw();
	}


}

START_NAMESPACE_DISTRHO

UI* createUI()
{
	return new sherman::SiL2Ui();
}

END_NAMESPACE_DISTRHO

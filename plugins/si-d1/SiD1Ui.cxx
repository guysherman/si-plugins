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

#include "SiD1Plugin.hxx"
#include "SiD1Ui.hxx"

namespace Art = SiD1Assets;
//namespace Art = DistrhoArtwork3BandEQ;

namespace sherman
{

SiD1Ui::SiD1Ui()
	: UI(Art::backgroundWidth, Art::backgroundHeight),
	backgroundImage(Art::backgroundData, Art::backgroundWidth, Art::backgroundHeight, GL_BGR)
{
	Image knobImage(Art::knobData, Art::knobWidth, Art::knobHeight);
	gainKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	gainKnob->setId(SiD1Plugin::paramGain);
	gainKnob->setAbsolutePos(15, 15);
	gainKnob->setRange(0.0f, 48.0f);
	gainKnob->setDefault(0.0f);
	gainKnob->setRotationAngle(240);
	gainKnob->setCallback(this);

	biasKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	biasKnob->setId(SiD1Plugin::paramBias);
	biasKnob->setAbsolutePos(115, 15);
	biasKnob->setRange(-1.0f, 1.0f);
	biasKnob->setDefault(0.0f);
	biasKnob->setRotationAngle(240);
	biasKnob->setCallback(this);

	distanceKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	distanceKnob->setId(SiD1Plugin::paramDistance);
	distanceKnob->setAbsolutePos(215, 15);
	distanceKnob->setRange(0.0f, 2.0f);
	distanceKnob->setDefault(1.0f);
	distanceKnob->setRotationAngle(240);
	distanceKnob->setCallback(this);

	programLoaded(0);
}


	// -------------------------------------------------------------------
	// DSP Callbacks

	void SiD1Ui::parameterChanged(uint32_t index, float value)
	{
		switch(index)
		{
			case SiD1Plugin::paramGain:
			gainKnob->setValue(value);
			break;
			case SiD1Plugin::paramBias:
			biasKnob->setValue(value);
			case SiD1Plugin::paramDistance:
			distanceKnob->setValue(value);
			break;
		}
	}


	void SiD1Ui::programLoaded(uint32_t index)
	{
		if (index != 0)
		{
			return;
		}

		gainKnob->setValue(0.0f);
	}

	// -------------------------------------------------------------------
	// Widget Callbacks

	void SiD1Ui::imageKnobDragStarted(ImageKnob* knob)
	{
		editParameter(knob->getId(), true);
	}

	void SiD1Ui::imageKnobDragFinished(ImageKnob* knob)
	{
		editParameter(knob->getId(), false);
	}

	void SiD1Ui::imageKnobValueChanged(ImageKnob* knob, float value)
	{
		setParameterValue(knob->getId(), value);
	}

	void SiD1Ui::onDisplay()
	{
		backgroundImage.draw();
	}


}

START_NAMESPACE_DISTRHO

UI* createUI()
{
	return new sherman::SiD1Ui();
}

END_NAMESPACE_DISTRHO

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

#include "SiD2Plugin.hxx"
#include "SiD2Ui.hxx"

namespace Art = SiD2Assets;
//namespace Art = DistrhoArtwork3BandEQ;

namespace sherman
{

SiD2Ui::SiD2Ui()
	: UI(Art::backgroundWidth, Art::backgroundHeight),
	backgroundImage(Art::backgroundData, Art::backgroundWidth, Art::backgroundHeight, GL_BGRA)
{
	Image knobImage(Art::knobData, Art::knobWidth, Art::knobHeight);
	gainKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	gainKnob->setId(SiD2Plugin::paramGain);
	gainKnob->setAbsolutePos(15, 17);
	gainKnob->setRange(0.0f, 48.0f);
	gainKnob->setDefault(0.0f);
	gainKnob->setRotationAngle(240);
	gainKnob->setCallback(this);

	slopeKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	slopeKnob->setId(SiD2Plugin::paramSlope);
	slopeKnob->setAbsolutePos(115, 17);
	slopeKnob->setRange(0.5f, 2.0f);
	slopeKnob->setDefault(1.0f);
	slopeKnob->setRotationAngle(240);
	slopeKnob->setCallback(this);

	levelKnob = new ImageKnob(this, knobImage, ImageKnob::Vertical);
	levelKnob->setId(SiD2Plugin::paramLevel);
	levelKnob->setAbsolutePos(215, 17);
	levelKnob->setRange(0.0f, 1.0f);
	levelKnob->setDefault(1.0f);
	levelKnob->setRotationAngle(240);
	levelKnob->setCallback(this);

	programLoaded(0);
}


	// -------------------------------------------------------------------
	// DSP Callbacks

	void SiD2Ui::parameterChanged(uint32_t index, float value)
	{
		switch(index)
		{
			case SiD2Plugin::paramGain:
			gainKnob->setValue(value);
			break;
			case SiD2Plugin::paramSlope:
			slopeKnob->setValue(value);
			break;
			case SiD2Plugin::paramLevel:
			levelKnob->setValue(value);
			break;
		}
	}


	void SiD2Ui::programLoaded(uint32_t index)
	{
		if (index != 0)
		{
			return;
		}

		gainKnob->setValue(0.0f);
	}

	// -------------------------------------------------------------------
	// Widget Callbacks

	void SiD2Ui::imageKnobDragStarted(ImageKnob* knob)
	{
		editParameter(knob->getId(), true);
	}

	void SiD2Ui::imageKnobDragFinished(ImageKnob* knob)
	{
		editParameter(knob->getId(), false);
	}

	void SiD2Ui::imageKnobValueChanged(ImageKnob* knob, float value)
	{
		setParameterValue(knob->getId(), value);
	}

	void SiD2Ui::onDisplay()
	{
		backgroundImage.draw();
	}


}

START_NAMESPACE_DISTRHO

UI* createUI()
{
	return new sherman::SiD2Ui();
}

END_NAMESPACE_DISTRHO

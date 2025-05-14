#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderImGui.h"
#include ".../../../../SpectralMorpher/vc2019/MidiUtils.h"

#include "cinder/audio/audio.h"
#include ".../../../../SpectralMorpher/vc2019/CustomFilter.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const int MAX_VOICES = 8;

class SpectralMorpherApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

	void mouseMove(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void keyUp(KeyEvent event) override;

private:
	ivec2 mousePos = ivec2();

	Color col = Color(0, 0, 1);

	int currentPressedNotes[8] = { -1,-1,-1,-1,-1,-1,-1,-1 };

	int currentOctave = 0;

	// audio nodes
	audio::GenOscNodeRef	mOsc[8];
	audio::GainNodeRef	mGain[8];
	audio::GainNodeRef		mOscAdder;
	audio::GainNodeRef	mMasterGain;

	audio::InputDeviceNodeRef		mInput;

	CustomFilterRef mCustomFilter;
	BiquadFilterRef mBiquad;
	FilterBankRef mFilterBank;


};

/// <summary>
/// Setup
/// </summary>
void SpectralMorpherApp::setup()
{
	auto ctx = audio::master();

	mOscAdder = ctx->makeNode(new audio::GainNode(1.0f));
	mMasterGain = ctx->makeNode(new audio::GainNode(0.7f));

	for (int i = 0; i < MAX_VOICES; i++) {
		mOsc[i] = ctx->makeNode(new audio::GenOscNode);
		mOsc[i]->setWaveform(audio::WaveformType::SAWTOOTH);

		mGain[i] = ctx->makeNode(new audio::GainNode(0));
		mOsc[i] >> mGain[i] >> mOscAdder;
	}

	mInput = ctx->createInputDeviceNode();
	mInput->enable();
	//mInput >> mOscAdder;

	mCustomFilter = ctx->makeNode(new CustomFilter());
	mBiquad = ctx->makeNode(new BiquadFilter());

	mFilterBank = ctx->makeNode(new FilterBank());

	mOscAdder >> mCustomFilter >> mFilterBank >> mMasterGain;
	mMasterGain >> ctx->getOutput();

	ctx->enable();
}


/// <summary>
/// Input Stuff
/// </summary>
/// <param name="event"></param>

void SpectralMorpherApp::mouseDown(MouseEvent event)
{

}


void SpectralMorpherApp::mouseMove(MouseEvent event) {
	mousePos = event.getPos();
	//mCustomFilter->setDrive(mousePos.y*0.1f);
	mFilterBank->setFreq(mousePos.x * 0.001f);
	mBiquad->setQ(mousePos.y * 0.01f);
}


void SpectralMorpherApp::keyDown(KeyEvent event)
{
	int note = MidiUtils::keyToNote(event.getChar()) + currentOctave * 12;
	if (note >= 0) {
		int result = MidiUtils::addNoteToPool(note, currentPressedNotes);

		if (result != -1) {
			mOsc[result]->setFreq(MidiUtils::midiToFreq(note));
			mOsc[result]->enable();
			mGain[result]->setValue(0.5f);


			console() << "Added note " << (note) << " to: " << result << std::endl;
			for (int i = 0; i < 8; i++) {
				console() << currentPressedNotes[i] << ", ";
			}
			console() << std::endl;
		}
	}

	if (event.getNativeKeyCode() == 38) currentOctave++;
	if (event.getNativeKeyCode() == 40) currentOctave--;

}

void SpectralMorpherApp::keyUp(KeyEvent event)
{
	int note = MidiUtils::keyToNote(event.getChar()) + currentOctave * 12;
	if (note >= 0) {
		int result = MidiUtils::removeNoteFromPool(note, currentPressedNotes);
		if (result != -1) {
			mOsc[result]->setFreq(0);
			mOsc[result]->disable();
			mGain[result]->setValue(0.0f);


			console() << "Removed note " << note << " from: " << result << std::endl;
			for (int i = 0; i < 8; i++) {
				console() << currentPressedNotes[i] << ", ";
			}
			console() << std::endl;
		}
	}

}

/// <summary>
/// Updating
/// </summary>
void SpectralMorpherApp::update()
{

}



void SpectralMorpherApp::draw()
{
	col.r = (float)mousePos.x / (float)toPixels(getWindowWidth());
	gl::clear(col);
}

CINDER_APP(SpectralMorpherApp, RendererGl)
#pragma once
#include "cinder/audio/audio.h"
#include "../src/Biquad.h"


using namespace ci;
using namespace std;

typedef std::shared_ptr<class SpectralHarmonizerEngine>	SpectralHarmonizerEngineRef;

#define MAX_FILTERBANK_SIZE 240

class SpectralHarmonizerEngine : public audio::Node {
public:
	SpectralHarmonizerEngine(const Format& format = Format()) : Node(format) { for (int i = 0; i < 2; i++) { biquad[i] = Biquad(); } }
	~SpectralHarmonizerEngine() {}
	void setPreGain(float gain);
	void setPitchShiftCt(float gain);
	void setFreqShiftHz(float gain);

private:
	void process(audio::Buffer* buffer);

	// parameters
	// pre effects
	float mPreGain = 1; // -100dB - 10dB
	float mPitchShiftCt = 0; // -2400 - 2400
	float mFreqShiftHz = 0; // -1000 - 1000

	// transients
	float preserveTransients = 0; // 0-1

	// filtering
	float filterResonance = 1; // 0 - 1
	float addHarmonicResonators = 0.3f; // 0-1
	
	// synthesis stuff
	Biquad filterbank[];
};


// Set Params

void SpectralHarmonizerEngine::setPreGain(float gain) {
	mPreGain = gain;
}

void SpectralHarmonizerEngine::setPitchShiftCt(float pitchShift) {
	mPitchShiftCt = pitchShift;
}

void SpectralHarmonizerEngine::setFreqShiftHz(float freqShift) {
	mFreqShiftHz = freqShift;
}


// Main Processing

void SpectralHarmonizerEngine::process(audio::Buffer* buffer)
{
	int numChannels = buffer->getNumChannels();
	int blockSize = buffer->getNumFrames();

	float* data = buffer->getData();

	for (int sample = 0; sample < blockSize; sample++) {
		float sample = (data[0 * blockSize + sample] + data[1 * blockSize + sample])/2.0f;

		// step 1: get transient response using rms peak detection
		// calc abs
		// lowpass
		// rms

		// step 2: apply pitch/frequency shift
		// pitch: use phase vocoder
		// freq: use hilbert transformer

		// step 3: apply pre gain
		sample *= mPreGain
		

		// step 4:
		// constant q transform using filterbank (bandpass for every note + some harmonics)
		// apply intensity mask (non parametric -> use spline based) -> multiply each band with intensity at that freq
		// if not in key -> pitch shift band up or down, so it fits into scale (needs a lot of pitch shifters so maybe inefficient)
		// Apply distortion to each band individually to create higher harmonics -> exciter
		// Apply feedback comb filter to resonate harmonics
		// Apply intensity mask to bands and add them together
		// or
		// apply intensity mask to filterbank of peak filters to boost in key frequencies and lessen out key frequencies
		// or
		// use delay based feedback resonators to boost in key frrquencies and cut out key frequencies

		// step 5: apply transients back using transient shaper
		// use compressor to reduce volume if over original envelope




		data[0 * blockSize + sample] = input;
		data[1 * blockSize + sample] = input;

		// channel based processing used later
		/*
		for (int ch = 0; ch < numChannels; ch++) {
			
		}
		*/
	}
}

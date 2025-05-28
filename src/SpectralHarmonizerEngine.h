#pragma once
#include "cinder/audio/audio.h"
#include "../src/Biquad.h"


using namespace ci;
using namespace std;

typedef std::shared_ptr<class BiquadFilter>	BiquadFilterRef;

class BiquadFilter : public audio::Node {
public:
	BiquadFilter(const Format& format = Format()) : Node(format) { for (int i = 0; i < 2; i++) { biquad[i] = Biquad(); } }
	~BiquadFilter() {}
	void setFreq(float drive);
	void setQ(float q);

private:
	void process(audio::Buffer* buffer);
	float mFreq = 1000;
	Biquad biquad[2];
};



inline void BiquadFilter::setFreq(float freq)
{
	for (int i = 0; i < 2; i++) {
		biquad[i].setFc(freq);
	}
}

inline void BiquadFilter::setQ(float q)
{
	for (int i = 0; i < 2; i++) {
		biquad[i].setQ(q);
	}
}



void BiquadFilter::process(audio::Buffer* buffer)
{
	int numChannels = buffer->getNumChannels();
	int blockSize = buffer->getNumFrames();

	float* data = buffer->getData();

	for (int sample = 0; sample < blockSize; sample++) {
		for (int ch = 0; ch < numChannels; ch++) {
			data[ch * blockSize + sample] = biquad[ch].process(data[ch * blockSize + sample]);
		}
	}
}

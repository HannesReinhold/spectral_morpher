#pragma once
#include "cinder/audio/audio.h"

using namespace ci;
using namespace std;

class CustomFilter : public audio::Node {
public:

private:
	void process(audio::Buffer* buffer);
};



void CustomFilter::process(audio::Buffer* buffer)
{
	int numChannels = buffer->getNumChannels();
	int blockSize = buffer->getNumFrames();

	for (int channel = 0; channel < numChannels; channel++) {

		float* ch = buffer->getChannel(channel);

		for (int sample = 0; sample < blockSize; sample++) {
			ch[sample] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 0.1f;
		}
	}
}

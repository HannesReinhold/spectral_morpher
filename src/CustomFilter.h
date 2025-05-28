#pragma once
#include "cinder/audio/audio.h"
#include "../src/Biquad.h"

using namespace ci;
using namespace std;


typedef std::shared_ptr<class CustomFilter>	CustomFilterRef;

class CustomFilter : public audio::Node {
public:
	CustomFilter(const Format& format = Format()) : Node(format) {}
	void setDrive(float drive);

private:
	void process(audio::Buffer* buffer);
	float mDrive = 1;
};



inline void CustomFilter::setDrive(float drive)
{
	mDrive = drive;
}

void CustomFilter::process(audio::Buffer* buffer)
{
	int numChannels = buffer->getNumChannels();
	int blockSize = buffer->getNumFrames();

	float* data = buffer->getData();

	for (int sample = 0; sample < blockSize; sample++) {
		for (int ch = 0; ch < numChannels; ch++) {
			data[ch * blockSize + sample] = std::atan(data[ch * blockSize + sample] * mDrive);
		}
	}
}




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


typedef std::shared_ptr<class FilterBank>	FilterBankRef;

class FilterBank : public audio::Node {
public:
	FilterBank(const Format& format = Format()) : Node(format) { for (int i = 0; i < 22; i++) { biquad[i] = Biquad(4, 0.5f, 40, 10); } }
	~FilterBank() {}
	void setFreq(float drive);
	void setQ(float q);

private:
	void process(audio::Buffer* buffer);
	float mFreq = 1000;
	Biquad biquad[22];
	float frequencies[22] = { 440.0f / 44100.0f, 493.88f / 44100.0f, 523.25f / 44100.0f, 587.33f / 44100.0f,659.26f / 44100.0f, 698.46f / 44100.0f,783.990f / 44100.0f,440.0f / 44100.0f*2, 493.88f / 44100.0f*2, 523.25f / 44100.0f*2, 587.33f / 44100.0f*2,659.26f / 44100.0f*2, 698.46f / 44100.0f*2,783.990f / 44100.0f*2 ,440.0f / 44100.0f *3, 493.88f / 44100.0f * 3, 523.25f / 44100.0f * 3, 587.33f / 44100.0f *3,659.26f / 44100.0f * 3, 698.46f / 44100.0f * 3,783.990f / 44100.0f * 3 ,440.0f / 44100.0f * 4 };
};



inline void FilterBank::setFreq(float freq)
{
	for (int i = 0; i < 22; i++) {
		biquad[i].setFc(frequencies[i]*freq);
	}
}

inline void FilterBank::setQ(float q)
{
	for (int i = 0; i < 22; i++) {
		biquad[i].setQ(q);
	}
}



void FilterBank::process(audio::Buffer* buffer)
{
	int numChannels = buffer->getNumChannels();
	int blockSize = buffer->getNumFrames();

	float* data = buffer->getData();

	for (int sample = 0; sample < blockSize; sample++) {
		for (int ch = 0; ch < 1; ch++) {
			//float sum = (static_cast <float>(rand()) / static_cast <float>(RAND_MAX)*2.0f-1.0f)*0.25f;
			float input = (data[ch * blockSize + sample] + data[1 * blockSize + sample]) * 0.5f;
			float sum = input;
			for (int i = 0; i < 22; i++) {
				sum = biquad[i].process(sum);
			}
			data[ch * blockSize + sample] = sum - input;
			data[1 * blockSize + sample] = sum - input;
		}
	}
}
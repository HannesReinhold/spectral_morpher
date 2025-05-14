#pragma once
#include "cinder/audio/audio.h"
#include "../../Biquad.h"

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
	FilterBank(const Format& format = Format()) : Node(format) { for (int i = 0; i < 2; i++) { biquad[i] = Biquad(2, 0.5f, 10, 1); } }
	~FilterBank() {}
	void setFreq(float drive);
	void setQ(float q);

private:
	void process(audio::Buffer* buffer);
	float mFreq = 1000;
	Biquad biquad[20];
	float frequencies[10] = { 440.0f / 44100.0f, 466.16f / 44100.0f, 493.88f / 44100.0f, 523.25f / 44100.0f,  554.37f / 44100.0f, 587.33f / 44100.0f, 622.25f / 44100.0f,659.26f / 44100.0f, 698.46f / 44100.0f, 739.99f / 44100.0f };
};



inline void FilterBank::setFreq(float freq)
{
	for (int i = 0; i < 10; i++) {
		biquad[i].setFc(frequencies[i] + freq / 44100.0f);
	}
}

inline void FilterBank::setQ(float q)
{
	for (int i = 0; i < 20; i++) {
		biquad[i].setQ(q);
	}
}



void FilterBank::process(audio::Buffer* buffer)
{
	int numChannels = buffer->getNumChannels();
	int blockSize = buffer->getNumFrames();

	float* data = buffer->getData();

	for (int sample = 0; sample < blockSize; sample++) {
		for (int ch = 0; ch < numChannels; ch++) {
			float sum = 0;
			for (int i = 0; i < 10; i++) {
				sum += biquad[ch].process(data[ch * blockSize + sample]);
			}
			data[ch * blockSize + sample] = sum / 2.0f;
		}
	}
}
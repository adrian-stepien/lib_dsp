#ifndef BIQUADFILTER_H_INCLUDED
#define BIQUADFILTER_H_INCLUDED

#ifndef PI
#define PI 3.14159265359
#endif

#include <vector>
#include "../core/buffer.h"

using namespace dsp;
using std::vector;

namespace dsp
{

class BiQuadFilter {
public: 
	/*
	Constructor. Initializes the sample rate and center frequency.
	Creates a series of Buffers and pushes them into the vectors for each channel.
	*/
	BiQuadFilter(int numChannels, int centerFrequency, double sampleRate);
	
	/*
	Calculates and returns the next output sample based on the sample that is input.
	Channel specifies which channel is being processed.
	*/
	float nextSample(float sample, int channel);
	
	void setSampleRate(double sampleRate) { fs = sampleRate; }
	double getSampleRate() { return fs; }
	
	/*
	Call any time the center frequency is changed. All coefficients are recalculated.
	*/
	virtual void update(float fc, float fs, float G = 0);
	
	int numChannels;
	double fs; //Sample Rate
	int fc; //Center Frequency

	float a0, a1, a2; //Input coefficients
	float b1, b2; //Output coefficients
	float c0, d0; //Dry/Wet coefficients
	
private:
	//Holds a number of buffers equal to the number of channels.
	//Each buffer only has two samples and represents the input buffer.
	vector<Buffer> inputBuffer;
	
	//Holds a number of buffers equal to the number of channels.
	//Each buffer only has two samples and represents the output buffer.
	vector<Buffer> outputBuffer;
};

BiQuadFilter::BiQuadFilter(int numChannels, int centerFrequency, double sampleRate){
	fc = centerFrequency;
	BiQuadFilter::numChannels = numChannels;
	fs = sampleRate;
	for (int i = 0; i < numChannels; i++){
		Buffer *b = new Buffer(2);
		inputBuffer.push_back(*b);
		b = new Buffer(2);
		outputBuffer.push_back(*b);
	}
}

float BiQuadFilter::nextSample(float sample, int channel){
	float output = a0 * sample + a1 * inputBuffer[channel].getSample(0) + a2 * inputBuffer[channel].getSample(1)
				 - b1 * outputBuffer[channel].getSample(0) - b2 * outputBuffer[channel].getSample(1);
				 
	outputBuffer[channel].setSample( outputBuffer[channel].getSample(0),1);
	outputBuffer[channel].setSample(output, 0);
	inputBuffer[channel].setSample(inputBuffer[channel].getSample(0), 1);
	inputBuffer[channel].setSample(sample, 0);
	
	output = 0.5 * c0 * output + 0.5 * d0 * sample;	
	return output;
}

void BiQuadFilter::update(float fc, float fs, float G){}

}//end namespace dsp
#endif

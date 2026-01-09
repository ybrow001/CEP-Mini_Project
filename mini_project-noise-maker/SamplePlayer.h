#pragma once
#include "cstdlib"
#include "vector"
#include "libraries/AudioFile/AudioFile.h"

class SamplePlayer {
	public:
		SamplePlayer(); // constructor - can be used to initialise values for private member variables
		bool load(std::string sample);
		float play(unsigned int channel);
		void setGain(float gainIn);
	private:
		std::string sample;
		std::vector<std::vector<float>> sampleData;
		unsigned int readPointer;
		float gain;
};
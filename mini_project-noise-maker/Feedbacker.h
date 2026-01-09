#pragma once 
#include "HighpassFilter.h"
#include "LowpassFilter.h"

class Feedbacker {
	public: 
		Feedbacker();
		float process(float input, float gainMult, unsigned int delaySamples, float clipLimit);
	
	private:
		float delayBuffer[4410] = {0.0f}; // array with [_] items, items initialised as {_}
		int writeIndex = 0;
		const int bufferSize = 4410;
};
#include "Feedbacker.h"
#include "HighpassFilter.h"
#include "LowpassFilter.h"
#include "cmath"
#include "cstdlib"

// constructor
Feedbacker::Feedbacker() {};

float Feedbacker::process(float input, float gainMult, unsigned int delaySamples, float clipLimit) {
	int readIndex = (writeIndex + bufferSize - delaySamples) % bufferSize; // determine position of readIndex relative to writeIndex
	float delayedFeedback = delayBuffer[readIndex]; // assign delayedFeedback signal from delay buffer
	
	// ensure gain multiplier is within safe boundaries
	if(gainMult < 0.0f) gainMult = 0.0f;
	if(gainMult > 1.1f) gainMult = 1.1f; // allow it to be "turned up to 11"
	
	// IIR feedback formula = y(n) = x(n) + g * y(n−1)
	float y = input + (gainMult * delayedFeedback);
	
	// apply soft clipping and saturation to signal using hyperbolic tangent function
	y = tanhf(1.0f * y); // increasing float value applies clipping more aggresively
	
	// polynomial soft clipping
	y = y / (1.0f + fabsf(y));
	
	// hard clip for distortion
	y = fminf(fmaxf(y, -clipLimit), clipLimit); 
	
	// update delay
	delayBuffer[writeIndex] = y; // write to buffer
	writeIndex = (writeIndex + 1) % bufferSize; // increment & wrap circular buffer
	
	// hard clip lower and upper limits of the signal for safety
	y = fminf(fmaxf(y, -1.0f), 1.0f); 
	
	return y;
};
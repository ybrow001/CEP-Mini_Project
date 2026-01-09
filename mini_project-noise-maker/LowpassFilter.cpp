#include "LowpassFilter.h"
#include <cmath>

// constructor
LowpassFilter::LowpassFilter() {};

// setup method to build filter
void LowpassFilter::initialise(float cutoff, float sampleRateIn) {
	sampleRate = sampleRateIn;
	
    // calculate coefficients
    float q = 0.707f; // Butterworth Q value, maximally flat resonance
    float omega = 2.0f * M_PI * cutoff/sampleRate;
    float alpha = sin(omega) / (2.0f * q);
    
    float b0 = 1.0f - cos(omega) / 2.0f;
    float b1 = 1.0f - cos(omega);
    float b2 = 1.0f - cos(omega) / 2.0f;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cos(omega);
    float a2 = 1.0f - alpha;
    
    // normalise coefficients
    this->b0 = b0/a0;
    this->b1 = b1/a0;
    this->b2 = b2/a0;
    this->a1 = a1/a0;
    this->a2 = a2/a0;
    
    // reset inputs and outputs
    x1 = 0.0f;
    x2 = 0.0f;
    y1 = 0.0f;
    y2 = 0.0f;
}

// process input sample, output processed sample
float LowpassFilter::process(float x, float dryGain, float wetGain) {
	// digital biquad difference equation using calculated coefficients and input
	// b_ are input, a_ are output
	
	float y = b0*x + b1*x1 + b2*x2 - a1*y1 - a2*y2;
	
	// update i/os with previous values
	x2 = x1;
	x1 = x;
	y2 = y1;
	y1 = y;
	
	// apply dry & wet gain
    return (x * dryGain) + (y * wetGain);
};

// update cutoff frequency
void LowpassFilter::update(float newCutoff, float newQ) {
	// recalculate filter
	float q = newQ;
	float omega = 2.0f * M_PI * newCutoff/sampleRate;
    float alpha = sin(omega) / (2.0f * q);
    
    float b0 = 1.0f - cos(omega) / 2.0f;
    float b1 = 1.0f - cos(omega);
    float b2 = 1.0f - cos(omega) / 2.0f;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cos(omega);
    float a2 = 1.0f - alpha;
    
    // normalise coefficients
    this->b0 = b0/a0;
    this->b1 = b1/a0;
    this->b2 = b2/a0;
    this->a1 = a1/a0;
    this->a2 = a2/a0;
};

void LowpassFilter::reset() {
	x1 = 0.0f; 
	x2 = 0.0f;
	y1 = 0.0f;
	y2 = 0.0f;	
};

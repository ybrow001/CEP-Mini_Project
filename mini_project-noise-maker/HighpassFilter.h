#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H

// Lowpass Biquad Filter

class HighpassFilter {
	public:
		HighpassFilter();
		void initialise(float cutoff, float sampleRateIn);
		float process(float x, float dryGain, float wetGain); // takes input audio
		void update(float cutoff, float newQ);
		void reset();
	private:
		unsigned int sampleRate;
		float b0,b1,b2,a1,a2; //filter coefficients
		float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f; // filter inputs and outputs
};

#endif
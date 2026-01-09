#include "SamplePlayer.h"
#include "cstdlib"
#include "vector"
#include "libraries/AudioFile/AudioFile.h"

SamplePlayer::SamplePlayer() {
	readPointer = 0;
	gain = 0.33f;
};

bool SamplePlayer::load(std::string sample) {
	sampleData = AudioFileUtilities::load(sample);
	readPointer = 0;
	
	return !sampleData.empty();
};

float SamplePlayer::play(unsigned int channel) {
	if(sampleData.empty()) {
    	return 0.0f;
	};
	
    // increment read pointer and wrap at end of audio file
    readPointer += 1;
    if(readPointer > sampleData[0].size()) {
    	readPointer = 0;
    };

    // wrap channel index
    float out = sampleData[channel%sampleData.size()][readPointer];
    
    return out * gain;
};

void SamplePlayer::setGain(float gainIn) {
	gain = gainIn;
};
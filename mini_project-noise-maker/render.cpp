/*
leaf cruncher - noise maker

WARNING: FEEDBACKER CLIP VALUES >1.0 MAY BE DANGEROUS.
- feedbacker clip is the limit of the hard clip applied to the feedbacker output, use values above 1.0 with caution.

- in order to use this noise maker a sample must be loaded into the sample player.
- it operates in a similar manner to no-input mixing. 
- changes to any of the parameters presented in the GUI will alter the sound of the audio output and noise quality.
- sounds can range from raw playback of the audio file, to intense droning or chaotic noise.
- experiment and have fun sculpting noise!
*/

#include <Bela.h>
#include <vector>
#include <libraries/AudioFile/AudioFile.h>
#include "SamplePlayer.h"
#include "Feedbacker.h"
#include "HighpassFilter.h"
#include "LowpassFilter.h"
#include "libraries/Gui/Gui.h"
#include "libraries/GuiController/GuiController.h"

// global varibales

float previousLeftGain = 0.5f;
float previousRightGain = 0.5f;

// create instances of classes
SamplePlayer player;
Feedbacker feedbacker;
HighpassFilter hpf;
LowpassFilter lpf;
Gui gui;
GuiController guiController;

bool setup(BelaContext *context, void *userData){
	
	const unsigned int sampleRate = context->audioSampleRate;
	
	// hpf gui
	gui.setup(context->projectName);
	guiController.setup(&gui, "Noise Controls");
	guiController.addSlider("HPF Cutoff", 4000.0f, 60.0f, 16000.0f);
	guiController.addSlider("HPF Q Resonance", 0.707f, 0.5f, 5.0f);
	guiController.addSlider("HPF Mix Dry<->Wet", 0.5f, 0.0f, 1.0f);
	// lpf gui
	guiController.addSlider("LPF Cutoff", 4000.0f, 60.0f, 18000.0f);
	guiController.addSlider("LPF Q Resonance", 0.707f, 0.5f, 5.0f);
	guiController.addSlider("LPF Mix Dry<->Wet", 0.5f, 0.0f, 1.0f);
	// player GUI
	guiController.addSlider("Player Gain", 0.5f, 0.0f, 1.1f);
	// feedbacker GUI
	guiController.addSlider("Feedbacker Gain", 0.5f, 0.0f, 1.1f);
	guiController.addSlider("Feedbacker Delay (Samples)", 1103, 1, 4410);
	guiController.addSlider("Feedbacker Clip", 1.0f, 0.33f, 1.33f);
	guiController.addSlider("Pan L<->R", 0.5f, 0.0f, 1.0f);
	
	// set path for audio file and load audio into a source file buffer
	player.load("58553__alexbird__grasshoppers-and-wind1.wav");
	player.setGain(0.5f);
	
	// initialise filters, passing in cutoffs and sample rate
	hpf.initialise(60.0f, sampleRate);
	lpf.initialise(8000.0f, sampleRate);
	
	return true;
}

void render(BelaContext *context, void *userData) { // audio block rate loop

	// block rate panning
    float targetLeftGain = 1.0f - guiController.getSliderValue(10);
    float targetRightGain = guiController.getSliderValue(10);
	// calculate steps to interpolate between pan values in samples
	float panInterpolationStepL = (targetLeftGain - previousLeftGain) / context->audioFrames;
	float panInterpolationStepR = (targetRightGain - previousRightGain) / context->audioFrames;

    for(unsigned int n = 0; n < context->audioFrames; n++) { // sample rate loop
    	
    	// update parameters using sliders directly or using variabes
    	hpf.update(
    		guiController.getSliderValue(0), // cutoff
    		guiController.getSliderValue(1) // Q
    	);
    	float hpfDryGain = 1.0f - guiController.getSliderValue(2);
    	float hpfWetGain = guiController.getSliderValue(2);
    	
    	lpf.update(
    		guiController.getSliderValue(3),
    		guiController.getSliderValue(4)
    	);
    	float lpfDryGain = 1.0f - guiController.getSliderValue(5);
    	float lpfWetGain = guiController.getSliderValue(5);
    	
    	player.setGain(guiController.getSliderValue(6));
    	
    	float feedbackerGain = guiController.getSliderValue(7);
    	float feedbackerDelay = guiController.getSliderValue(8);
    	float feedbackerClip = guiController.getSliderValue(9);
    	
    	// sample rate panning
    	float currentLeftGain = previousLeftGain + (n * panInterpolationStepL);
    	float currentRightGain = previousRightGain + (n * panInterpolationStepR);
    	
    	// call play method of player
    	float dryOut = player.play(0);
    	dryOut = player.play(1);
    	
    	// process audio with filters
    	// dry and wet levels included in filters for control over noise output
    	dryOut = hpf.process(dryOut, hpfDryGain, hpfWetGain);
    	dryOut = lpf.process(dryOut, lpfDryGain, lpfWetGain);
    	
    	// process audio with feedbacker & output
    	float out = feedbacker.process(dryOut, feedbackerGain, feedbackerDelay, feedbackerClip);
    	
    	// pan audio using left & right channels
    	audioWrite(context, n, 0, out * currentLeftGain);
    	audioWrite(context, n, 1, out * currentRightGain);
    }
    
    // update previous pan gain values
    previousLeftGain = targetLeftGain;
    previousRightGain = targetRightGain;
}

void cleanup(BelaContext *context, void *userData) {}
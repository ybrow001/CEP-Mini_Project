Yew Brown <br>
BSc Creative Computing <br>
Creative Embedded Programming <br>
12.01.2026 <br>

This document along with the Bela project code can be found on GitHub:  
https://github.com/ybrow001/CEP-Mini_Project

# Supporting Documentation for Bela Mini Project Submission

## Introduction & General Specifications

In this document I will detail the function of, my motivations for and possible further developments for this Bela project. I will also give a technical overview of the code and an explanation of how it can be run.  

This project was developed using the Bela IDE and are designed to run on a Bela, so one is required for it to be used and the code altered.  
Alongside the Bela itself, it requires a way of listening to the audio output via an audio adapter using the 3 pin Molex-style connector on the Bela.  

The project can be used by uploading the files in to the Bela IDE, running and opening the GUI window from the Bela IDE. From there the output sound can be sculpted by altering slider values and experimenting!  

If the user wishes to change the sound file, an new WAV file can be uploaded into the project folder. Then the file name must replaced in the *.load()* method of the *SamplePlayer* class, in the *setup()* function of the *render.cpp* file.  

## *field thresher*

### Function, Motivation & Possible Further Extensions

Leaf Cruncher is a noise generating software instrument. It creates noise via the processing of an audio sample, utilising feedback, delay, clipping, highpass and lowpass filters. Functioning similarly to the use of no-input mixing - a feedback loop created by inputing an analogue mixer into itself, as a method of generating playable harsh noise. 

The inspiration for this project came from my own music practice. Over the last few year, I have tried various ways of creating harsh noise. Including digital audio workstation (DAW) plug-ins and analogue non-input mixing<sup>1</sup>. 
During this creative work, I found a lack of techniques within the digital methods I was using, that enabled me to have a greater degree of control over the qualities of the noise being produced. In particular, ways of processing input sounds so that possible outputs would provide a wide range of noise quality, from soft to harsh noise, derived from the input audio. Something that could prove to be a dynamic and interesting sound design and performance instrument.  

*Further Developments*:

- Mixing of multiple audio files.

Mixing features would allow the user to provide multiple sound sources and mix them together in various ways, depending on implementation. 
The simplest implementation would feature individual audio player output levels for each audio source, possible controlled by a separate GUI interface to the Noise Controls. These levels could then after mixing, be summed combined into a single dry signal and processed through the filters and feedback objects. 
More complex implementations could involve processing through filter and feedback classes and panning applied to each each individual sound source, before summing each signal together into a single post-processing signal to output. Following the summing of the signal, further processing could also be added. These options would make a much more dynamic sound and greater user control.

- Processing of live audio input.

Handling of live input would also be a great addition to the programme, especially for use cases in live performance and sound production. Taking advantage of the Bela's real time processing capabilities. 
In combination with pre-loaded sound sources, it could be a very powerful and dynamic instrument.
A possible method of implementation could be use of a hardware audio input source, through the Bela's analogue inputs, such as a jack lead or microphone. The signal from this external input could then be processed by the Bela through use of the same classes and processes as pre-loaded audio. With the potential difference of handling the signal in a slightly different way, when using a buffer to create a margin for error and slower processing speeds, as the signal is only known at the moment of input. 
Further research into the processing of live audio input, will be needed to full understand how it might be executed.

- Hardware controls and features.

Similarly to live input, hardware controls for the programme would be a good addition for user interaction, in particular for live interation, tactile experience and precise motor control. 
An obvious be desirable implementation would be the replacement of GUI sliders with physical potentiator knobs, connected through the Bela's analogue pins and using its analogue to digital signal conversion processes.
However other more experimental approaches such as the use of the Trill sensor or a photoresistor, would be very interesting ways of interacting with the programme's control parameters.

All of the above improvements would greatly increase the programme's usability for my own creative practice. They would likely turn it into something I would readily use for both sound design and performance, filling the previously identified gap sound production gap in my practice.

### Technical Explanation

The programming is comprised of multiple classes handing different parts of audio processing, which are implemented in the main *render.cpp* file. In conjunction with one another, these classes are able to create playable, varied and dynamic noise from an audio file.  

These classes include:  
- SamplePlayer - simple audio player, outputting pre-loaded audio files.
- Feedbacker - the feedback, sound distortion and safety core of the programme.
- HighpassFilter (HPF) - implementing a biquad highpass filter.
- LowpassFilter (LPF) - implementing a biquad lowpass filter.

#### *SamplePlayer*

The *SamplePlayer* class was borrowed from the Creative Embedded Programming, Week #3, Lab 0 task - Loading and Playing Samples, Using a Circular Buffer for Delay. 

It loads an audio file into a buffer, linked via an appropriate file pathway in the *.load()* method of the class. 
Calling on the *.play()* method within the sample rate loop in the render.cpp file, a read pointer is incremented through the indices of the buffer. By outputting the data from from the read pointer and passing this signal to the audioWrite() function in the same render.cpp loop, the audio output of the player made audible. 
The read pointer is also wrapped back around to the start of the buffer once it reaches the end, to avoid error from reading indices that do not exist.

#### *Feedbacker*

The *Feedbacker* is central to the function of this programme. It takes the audio signal output by the sample player, following processing by the high- and low-pass filters, and uses it as the source for noice generation. 
This class can be broken down into three sub-features: short delay and comb filter, feedback, and clipping. These produce the noise output by delaying the audio input, feeding it back into itself and applying clipping as a source of non-linearities and a safety feature.  

The delay and feedback work together as an Infinite Impulse Response (IIR) filter<sup>2</sup>. The delay features extremely short delay times measured in samples, from 1 sample up to 4,410 samples, or 1/10 of a second. This short delay time creates a comb filter<sup>3</sup> through the introduction of constructive and destructive interference to the signal, resulting in audible resonant frequencies in the noise output. 
Together the short feedback loop and comb filter resonances produce outputs, such as instant or near instant feedback and resonant frequencies, similar to those encountered in no-input mixing.  

Clipping is used alongside the IIR delay/comb filter system. It is implemented is four stages: soft clipping, using a hyperbolic tangent (tanh) function, soft polynomial clipping, hard clipping before writing to the delay buffer and hard clipping for safety before outputting.
These clippers introduce non-linearity, colour and distortion to the output signal. Non-linearity prevents infinite feedback loops and allows the system to maintain a state of contained chaos, in much the same way as no-input mixing. 
The soft clipping of the tanh function can provide saturation resembling analogue systems and the polynomial clipping can provide a harsher, more digital sound. Used alongside one another, they introduce variation and chaos into the system producing dynamic results.
The first hard clipper is used to produce the harsh distortion and "cutting out" typically associated with clipping, when applied very aggressively, and lets through extra hot signals internally when opened fully. It also serves to contain the output signal of this class within relatively safe limits.
The final hard clipper exists primarily as a safety feature, keeping the output signal's amplitude within the range of -1.0 to 1.0. Secondarily serving as a source for further harsh distortion.  

GUI sliders allow the user to alter the feedback gain, delay time and clipping thresholds of the first hard clipper.  

#### *HPF*

The *HPF* class applies filtering via application of formulae for calculating HPF coefficients and use of the digital biquad difference equation, as found in the DSP Cookbook<sup>4</sup>. These calculations take place in the *.initialisation()*, *.process()* and *.update()* methods, determining the setting of cutoff and Q factor values and the processing and outputting of filtered frequencies.  

For the *.process()* method, dry and wet gain arguments are add alongside the input signal. These values allow for the mixing of the raw input (dry signal) and filtered output (wet signal) at the time of output. Providing the user with more control over the total output sound of the programme, and more possibilities for sound design and noise generation.  

Through GUI sliders the user is able to manipulate the cutoff frequency, dry/wet signal mix and the Q factor of the filter.

#### *LPF*

The *LPF* is applied and works and almost identically to the *HPF* class. A small exception being the order of implementation the *HPF* is applied first in *render.cpp*, the *LPF* is then applied to that filtered signal.  

#### *render.cpp*

In the *render.cpp* file, the above classes are initialised in the *setup()* loop and implemented in the *render()* and nested sample rate loops.
After processing and playing the audio signal via the *SamplePlayer* class, the signal is processed through the *HPF* and *LPF* classes, before finally passing to the *Feedbacker* class.  

This signal flow allows the original audio to necessarily be generated at the beginning. The signal is then filtered before noise processing, altering the output of subsequent noise output, by effecting the inout signal into *Feedbacker*. As well as creating the possibility for clean, plainly filtered and delayed audio being output with the right *Feedbacker* settings.  
Finally, *Feedbacker* is applied last at the end of the signal chain, before outputting through the *audioWrite()* function. It is placed last, as this allows any previous alterations to the sound of the audio signal to be fed into the noise generation processes. It also necessarily places the final hard clipper as the final process in the signal chain before output - this is extremely important as a safety feature, protecting both hardware and hearing from damage.  

Lastly, two other processes are carried out in the *render.cpp* file: the implementation of GUI sliders via the Gui and GuiController classes, and interpolation of pan values to prevent clicking and audible artifacts when adjusting the panning of the output.  
GUI sliders allow the user to adjust parameters of the *SamplePlayer*, *HPF*, *LPF* and *Feedbacker* classes and to adjust output pan values. 
The pan interpolation process calculates the difference between pan gain multiplier values in samples, using them to interpolate by step between the previous and next values, smoothing and avoiding audible artefacts.  

## References

1. SoundCloud source for personal harsh noise practice
  - https://soundcloud.com/alfthrith/sets/noise
  - More available here: https://soundcloud.com/alfthrith
<br>

2. Information about infinite impulse response filters on Wikipedia
  - https://en.wikipedia.org/wiki/Infinite_impulse_response#Implementation_and_design
<br>

3. Information about comb filters on Wikipedia
  - https://en.wikipedia.org/wiki/Comb_filter#Feedback_form
<br>

4. DSP Cookbook - Robert Bristow-Johnson, Raymond Toy
  - https://www.w3.org/TR/audio-eq-cookbook/#formulae
  - Used as reference for biquad highpass and lowpass filters.
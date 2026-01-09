Yew Brown <br>
BSc Creative Computing <br>
Creative Embedded Programming <br>
12.01.2026 <br>

# Supporting Documentation for Bela Mini Project Submission

## Introduction & General Specifications

In this document I will detail the function of, my motivations for and possible further developments for this Bela project. I will also give a technical overview of the code and an explaination of how it can be run.

This projectwas developed uing the Bela IDE and are designed to run on a Bela, so one is required for it to be used and the code altered. 
Alongside the Bela itself, it requires a way of listening to the audio output via an audio adapter using the 3 pin Molex-style connector on the Bela.

## Leaf Cruncher

### Function, Motivation & Possible Further Extensions

Leaf Cruncher is a noise generating software instrument. It creates noise via the processing of an audio sample, utilising feedback, delay, clipping, highpass and lowpass filters. Functioning similarly to the use of no-input mixing - a feedback loop created by inputing an analogue mixer into itself, as a method of generating playable harsh noise. 

The inspiration for this project came from my own music practice. Over the last few year, I have tried various ways of creating harsh noise. Including digital audio workstation (DAW) plug-ins and analogue non-input mixing<sup>1</sup>. 
During this creative work, I found a lack of techniques within the digital methods I was using, that enabled me to have a greater degree of control over the qualities of the noise being produced. In particular, ways of processing input sounds so that possible outputs would provide a wide range of noise quality, from soft to harsh noise, derived from the input audio. Something that could prove to be a dynamic and interesting sound design and performance instrument.

Further developments:

- Mixing of multiple audio files.

Mixing features would allow the user to provide multiple sound sources and mix them together in various ways, depending on implementation.  
The simplest implementation would feature individual audio player output levels for each audio source, possible controlled by a separate GUI interface to the Noise Controls. These levels could then after mixing, be summed combined into a single dry signal and processed through the filters and feedbacker objects.  
More complex implementations could involve processing through filter and feedbacker classes and panning applied to each each individual sound source, before summing each signal together into a single post-processing signal to output. Following the summing of signal, further processing could also be added. These options would make a much more dynamic sound and greater user control.

- Processing of live audio input.

Handling of live input would also be a great addition to the programme, especially for use cases in live performance and sound production. Taking advantage of the Bela's real time processing capabilities.  
In combination with pre-loaded sound sources, it could be a very powerful and dynamic instrument.
A possible method of implementation could be use of a hardware audio input source, through the Bela's analogue inputs, such as a jack lead or microphone. The signal from this external input could then be processed by the Bela through use of the same classes and processes as pre-loaded audio. With the potential difference of handling the signal in a slightly different way, when using a buffer to create a margin for error and slower processing speeds, as the signal is only known at the moment of input. 
Further research into the processing of live audio input, will be needed to full understand how it might be executed.

- Hardware controls and features.

Similarly to live input, hardware controls for the programme would be a good addition for user interaction, in particular for live interation, tactile experience and precise motor control.  
An obvious be desirable implementation would be the replacement of GUI sliders with physical potentiator knobs, connected through the Bela's analogue pins and using its analogue to digital signal conversion processes.
However other more experimental approaches such as the use of the Trill sensor or a photoresistor, would be very intersting ways of interacting with the programme's control parameters.

<br>

All of the above improvements would greatly increase the programme's usability for my own creative practice. They would likely turn it into something I would readily use for both sound design and performance, filling the previously identified gap sound production gap in my practice.

### Technical Explanation



## References

1. SoundCloud Source for Personal Harsh Noise Practice:
- https://soundcloud.com/alfthrith/sets/noise
- More available here: https://soundcloud.com/alfthrith

<br>

X. DSP Cookbook - Robert Bristow-Johnson, Raymond Toy:
- https://www.w3.org/TR/audio-eq-cookbook/#formulae
- Used as reference for the Biquad BPF (constant 0db peak) formula.
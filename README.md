![image](https://github.com/user-attachments/assets/b117ebc9-3bab-4e81-bc15-01de6b1b8c93)
# -Noise-Attenuation-using-Selective-Low-Pass-FIR-Filter-with-Audio-Visual-Verification

**Overview**

This project demonstrates noise attenuation using a low-pass FIR filter designed with the Hamming window method on an Arduino. It includes audio and visual feedback, making it an excellent tool for exploring signal processing and FIR filter design on a hardware platform.

**Features**
**Noise Attenuation:** Low-pass FIR filter to selectively attenuate unwanted high-frequency noise.
**Visual LED Indicators:** Three LEDs indicate the presence of specific input frequencies, allowing real-time monitoring.
**Audio Verification:** Buzzers for the noisy input and filtered output signals help verify the filter's performance through sound.

**Project Components**
**Arduino** (any compatible model)
**LEDs:** Visual indicators for input signals of different frequencies
**Buzzers:** Audio verification for noisy input and noise-attenuated output
**Breadboard and Resistors**
**FIR Filter:** Low-pass filter using Hamming window, configured to filter out frequencies above 12 Hz
Circuit Diagram
A simplified circuit diagram is shown below:
![Aurdino](https://github.com/user-attachments/assets/b51c5f10-f09e-454c-b3dc-cd9568ad0c8e)


**Code Explanation**
The code uses:

>>**Multi-frequency** signal synthesis to simulate a noisy signal.
>>**Hamming window** method for FIR filter design.
>>**LED controls** based on the amplitude and frequency of the signals.
>>**Buzzer feedback** to play tones that represent the input and filtered signals for audio verification.

**FIR Filter Details**
The FIR filter design uses a Hamming window, which applies a smooth transition to the filter coefficients, minimizing ripples in the frequency response. With a cutoff frequency of 12 Hz, the filter allows only lower frequencies to pass, reducing noise effectively.

**Getting Started**

**Setup Arduino:** Connect LEDs and buzzers according to the circuit diagram.

**Upload Code:** Load the provided Arduino sketch to start.

**Run the Project:** Observe LED indicators and listen to the buzzer tones for verification of noise attenuation.

**Usage**

Use the Serial Plotter to visualize the input and filtered signals.
LEDs and buzzers offer real-time visual and audio feedback of the filtering process.

**Future Enhancements**
Expand to test other filter types and configurations.
Integrate with additional sensors or wireless modules for remote monitoring.

// Parameters
const float freq1 = 5.0;          // Frequency of first sine wave (Hz)
const float freq2 = 10.0;         // Frequency of second sine wave (Hz)
const float freq3 = 15.0;         // Frequency of third sine wave (Hz)
const float sampleRate = 1000.0;  // Sampling rate (Hz)
const int numSamples = 32;        // Number of samples for FIR filter (must be odd)
const float cutoffFreq = 12.0;    // Cutoff frequency for FIR filter

const float scalingFactor = 10.0;  // Increased scaling factor for better output visibility

// Pin definitions for LEDs
const int inputLED1 = 3;   // LED for input signal 1 (5 Hz)
const int inputLED2 = 4;   // LED for input signal 2 (10 Hz)
const int inputLED3 = 5;   // LED for input signal 3 (15 Hz)
const int outputLED1 = 6;  // LED for output signal 1 (after filter)
const int outputLED2 = 7;  // LED for output signal 2 (after filter)
const int outputLED3 = 8;  // LED for output signal 3 (after filter)

// Add pin for the buzzers
const int inputBuzzerPin = 9;    // Buzzer for input signal
const int outputBuzzerPin = 10;  // Buzzer for output signal

// Frequencies for buzzer tones (in Hz)
const int buzzerInputFreq = 500;  // Fixed tone for input signal

// Timing control for buzzers
unsigned long buzzerStartTime = 0;
const unsigned long buzzerDuration = 1500;  // 1.5 seconds duration

bool inputBuzzerActive = true;

// FIR filter coefficients
float firCoeffs[numSamples];
float inputSignal[numSamples] = { 0 };  // Initialize input signal array to zero

// Time tracking
unsigned long lastSampleTime = 0;
float twoPiF1 = 2.0 * PI * freq1;
float twoPiF2 = 2.0 * PI * freq2;
float twoPiF3 = 2.0 * PI * freq3;

// Function to generate the combined sine wave with controlled Gaussian noise
float generateSineWave() {
  float time = millis() / 1000.0;                     // Time in seconds
  float sine1 = sin(twoPiF1 * time) * scalingFactor;  // Scale the amplitude
  float sine2 = sin(twoPiF2 * time) * scalingFactor;  // Scale the amplitude
  float sine3 = sin(twoPiF3 * time) * scalingFactor;  // Scale the amplitude

  // Turn on/off input LEDs based on signal presence
  digitalWrite(inputLED1, sine1 > 0 ? HIGH : LOW);
  digitalWrite(inputLED2, sine2 > 0 ? HIGH : LOW);
  digitalWrite(inputLED3, sine3 > 0 ? HIGH : LOW);

  // Generate Gaussian noise
  float noise = random(-250, 250) / 100.0;  // Range -2.5 to +2.5

  // Summing the three sine waves and adding noise
  float summedSignal = sine1 + sine2 + sine3 + noise;

  // Limit the output to a range for better visualization
  return constrain(summedSignal, -30, 30);
}

// Function to design a FIR filter using the window method (low pass)
// Adjusted FIR filter design function
void designFIRFilter(float cutoffFreq, float sampleRate) {
  float normalizedCutoff = cutoffFreq / (sampleRate / 2);  // Normalized cutoff
  for (int i = 0; i < numSamples; i++) {
    if (i == (numSamples - 1) / 2) {
      firCoeffs[i] = normalizedCutoff;  // Main lobe
    } else {
      int k = i - (numSamples - 1) / 2;
      firCoeffs[i] = sin(PI * normalizedCutoff * k) / (PI * k);
    }
    // Apply Hamming window
    firCoeffs[i] *= (0.54 - 0.46 * cos(2 * PI * i / (numSamples - 1)));
  }
}


// Function to apply FIR filter
float applyFIRFilter(float newSample) {
  // Shift samples to the right
  for (int i = numSamples - 1; i > 0; i--) {
    inputSignal[i] = inputSignal[i - 1];
  }
  inputSignal[0] = newSample;

  // Apply the FIR filter
  float output = 0.0;
  for (int i = 0; i < numSamples; i++) {
    output += inputSignal[i] * firCoeffs[i];
  }
  return output;
}

void setup() {
  Serial.begin(115200);  // Set the baud rate

  // Initialize LED pins as output
  pinMode(inputLED1, OUTPUT);
  pinMode(inputLED2, OUTPUT);
  pinMode(inputLED3, OUTPUT);
  pinMode(outputLED1, OUTPUT);
  pinMode(outputLED2, OUTPUT);
  pinMode(outputLED3, OUTPUT);

  // Initialize buzzer pins
  pinMode(inputBuzzerPin, OUTPUT);
  pinMode(outputBuzzerPin, OUTPUT);

  // Design FIR filter
  designFIRFilter(cutoffFreq, sampleRate);
}

void loop() {
  // Sampling based on sample rate
  if (millis() - lastSampleTime >= (1000 / sampleRate)) {
    lastSampleTime = millis();

    // Generate the input signal (sum of 3 sine waves with noise)
    float input = generateSineWave();

    // Apply FIR filter
    float output = applyFIRFilter(input);

    // Toggle buzzer sounds based on timing
    if (inputBuzzerActive) {
      tone(inputBuzzerPin, buzzerInputFreq);  // Sound input buzzer with fixed frequency (500 Hz)
      // Play the input buzzer for 1.5 seconds
      if (millis() - buzzerStartTime >= buzzerDuration) {
        noTone(inputBuzzerPin);
        inputBuzzerActive = false;
        buzzerStartTime = millis();  // Reset time for the output buzzer
      }
    } else {
      // Analyze the filtered output to control output LEDs
      bool signal1Present = abs(sin(twoPiF1 * (millis() / 1000.0))) > 0.1;
      bool signal2Present = abs(sin(twoPiF2 * (millis() / 1000.0))) > 0.1;
      bool signal3Present = abs(sin(twoPiF3 * (millis() / 1000.0))) > 0.1;

      // Output LED control based on the filtered signal
      digitalWrite(outputLED1, (output > 0 && freq1 <= cutoffFreq) ? HIGH : LOW);
      digitalWrite(outputLED2, (output > 0 && freq2 <= cutoffFreq) ? HIGH : LOW);
      digitalWrite(outputLED3, (output > 0 && freq3 <= cutoffFreq) ? HIGH : LOW);

      // Output buzzer frequency based on filtered output signal amplitude
      int buzzerToneFreq = map(abs(output), 0, 30, 200, 1200);  // Map output signal to buzzer frequency range
      tone(outputBuzzerPin, buzzerToneFreq);                    // Play output buzzer with variable frequency
      if (millis() - buzzerStartTime >= buzzerDuration) {
        noTone(outputBuzzerPin);
        inputBuzzerActive = true;    // Restart the cycle
        buzzerStartTime = millis();  // Reset time for the input buzzer
      }
    }

    // Print input and output values for debugging or Serial Plotter
    Serial.print(input);     // Print noisy input signal
    Serial.print(" ");       // Space separates signals
    Serial.println(output);  // Print filtered output signal
  }
}
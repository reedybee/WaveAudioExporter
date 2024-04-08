#include <wavExporter.h>

int main() {
	int32 numChannels = 1;
	int32 sampleRate = 41000;
	int32 numSeconds = 5;

	int32 numSamples = numChannels * sampleRate * numChannels;

	float phase = 0.0f;
	// create the sample buffer of the wav file
	float* sineData = new float[numSamples];
	// set the audio's data
	for (int i = 0; i < numSamples; i++) {
		// populate the sine audio data.
		sineData[i] = SineOscillator(phase, 420, sampleRate);
	}
	// create the wav files for each oscillator
	WriteWavFile("sine", sineData, numSamples, numChannels, sampleRate);
	// delete the data pointer to avoid leaking memory/heap
	delete[] sineData;
	// and youre done! the executable should have spit out a .wav file that sounds... interesting.
	return 1;
}
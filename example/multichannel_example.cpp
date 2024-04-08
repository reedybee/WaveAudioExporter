#include <wavExporter.h>

int main() {
	int32 numChannels = 2;
	int32 sampleRate = 41000;
	int32 numSeconds = 5;

	int32 numSamples = numChannels * sampleRate * numSeconds;

	float phase = 0.0f;
	float* data = new float[numSamples];
	// populates the data buffer;
	for (int i = 0; i < numSamples; i++) {
		// channels flip flop
		if (i % 2 == 0)
			data[i] = SineOscillator(phase, 300, sampleRate);
		else
			data[i] = SineOscillator(phase, 410, sampleRate);
	}
	WriteWavFile("multichannel", data, numSamples, numChannels, sampleRate);
	delete[] data;
	return 1;
}
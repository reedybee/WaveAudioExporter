#include <wavExporter.h>

int main() {
	int32 numChannels = 1;
	int32 sampleRate = 41000;
	int32 numSeconds = 5;

	int32 numSamples = numChannels * sampleRate * numSeconds;
	// each note of the music
	uint16 notes[] = {
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
		420, 420, 100, 600, 100,
	};

	float phase = 0.0f;
	float* data = new float[numSamples];
	// populate the audio buffer
	for (int i = 0; i < numSamples; i++) {
		// determines which note we are on
		int note = i * 4 / sampleRate;
		// populates the audios index with the note using a sine wav
		data[i] = SineOscillator(phase, notes[note], sampleRate);
	}
	// create a wav file with the music data.
	WriteWavFile("music", data, numSamples, numChannels, sampleRate);
	delete[] data;
	return 1;
}
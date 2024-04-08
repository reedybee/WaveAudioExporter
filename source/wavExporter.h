// welcome to reedybee's wav exporter, a completely single threaded, CPU wav audio format exporter.
// just include this header wherever you might want to use any function and enjoy!

#pragma once
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

typedef signed	 char	int8;
typedef signed	 short	int16;
typedef signed	 int	int32;
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

// clamps value between min and max
#define CLAMP(value,min,max) {if (value < min) { value = min; } else if(value > max) { value = max; }}

// basic sine oscillator.
static float SineOscillator(float& phase, float frequency, float sampleRate) {
	phase += (float)M_PI * frequency / sampleRate;
	while (phase >= 2 * (float)M_PI)
		phase -= 2 * (float)M_PI;
	while (phase < 0)
		phase += 2 * (float)M_PI;
	return sin(phase);
}
// basic sawtooth oscillator.
static float SawtoothOscillator(float& phase, float frequency, float sampleRate) {
	phase += frequency / sampleRate;
	while (phase > 1.0f)
		phase -= 1.0f;
	while (phase < 0.0f)
		phase += 1.0f;
	return (phase * 2.0f) - 1.0f;
}
// basic square oscillator.
static float SquareOscillator(float& phase, float frequency, float sampleRate) {
	phase += frequency / sampleRate;
	while (phase > 1.0f)
		phase -= 1.0f;
	while (phase < 0.0f)
		phase += 1.0f;
	if (phase <= 0.5f)
		return -1.0f;
	else
		return 1.0f;
}
// basic triangle oscillator.
static float TriangleOscillator(float& phase, float frequency, float sampleRate) {
	phase += frequency / sampleRate;
	while (phase > 1.0f)
		phase -= 1.0f;
	while (phase < 0.0f)
		phase += 1.0f;
	float ret;
	if (phase <= 0.5f)
		ret = phase * 2;
	else
		ret = (1.0f - phase) * 2;
	return (ret * 2.0f) - 1.0f;
}

// converts a float into a signed 32bit integer.
static int32 ConvertFloatToAudioSample(float value) {
	value *= 2147483647.0f;
	CLAMP(value, -2147483648.0f, 2147483647.0f);
	return (int32)value;
}
// wav file header structure
struct WavHeader {
	// main chunk
	uint8 chunkID[4];
	uint32 chunkSize;
	uint8 format[4];

	// format sub chunk 1
	uint8 subChunk1ID[4];
	uint32 subChunk1Size;
	uint16 audioFormat;
	uint16 numChannels;
	uint32 sampleRate;
	uint32 byteRate;
	uint16 blockAlign;
	uint16 bitsPerSample;

	// data sub chunk 2
	uint8 subChunkID2[4];
	uint32 subChunk2Size;
};
// creates or opens a wav file and fills the file with the audio data.
static int WriteWavFile(const char* filename, float* data, int32 numSamples, int16 numChannels, int32 sampleRate) {
	// open or create file
	std::string name = std::string(filename).append(".wav");
	FILE* file;
	int error = fopen_s(&file, name.c_str(), "w+b");
	if (error != 0) {
		printf("Failed to open/create file %s\n", name.c_str());
		return 0;
	}

	int32 bitsPerSample = 8 * sizeof(int32);
	int32 dataSize = numSamples * sizeof(int32);

	WavHeader wav;

	// fill main header
	memcpy(wav.chunkID, "RIFF", 4);
	wav.chunkSize = dataSize + 36;
	memcpy(wav.format, "WAVE", 4);

	// fill sub chunk 1
	memcpy(wav.subChunk1ID, "fmt ", 4);
	wav.subChunk1Size = 16;
	wav.audioFormat = 1;
	wav.numChannels = numChannels;
	wav.sampleRate = sampleRate;
	wav.byteRate = sampleRate * numChannels * bitsPerSample / 8;
	wav.blockAlign = numChannels * bitsPerSample / 8;
	wav.bitsPerSample = bitsPerSample;
	
	// fill sub chunk 2
	memcpy(wav.subChunkID2, "data", 4);
	wav.subChunk2Size = dataSize;

	// write header 
	fwrite(&wav, sizeof(WavHeader), 1, file);
	// past this point is the audio data itself
	int32* wavData = new int32[numSamples];
	// write the data input from function to the wav file;
	for (int i = 0; i < numSamples; i++)
		wavData[i] = ConvertFloatToAudioSample(data[i]);
	fwrite(data, dataSize, 1, file);
	delete[] wavData;

	fclose(file);
	return 1;
}
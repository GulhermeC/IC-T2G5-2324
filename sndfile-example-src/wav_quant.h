#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <fstream>

class WAVQuant {
  	private:
	std::vector<short> quantSamples;

  public:
	WAVQuant() {
        quantSamples.resize(0);
	}

    // Sets given amount of LSB to zero
    void quantBits(const std::vector<short>& samples, uint numBits) {
        for (auto s : samples) {
			short quantized = s & (0xFFFF << numBits);
            quantSamples.push_back(quantized);
        }
    }

    // Quantize by given number of levels
    void quantLevels(const std::vector<short>& samples, uint numlevels) {
		uint delta = (1 << 16) / numlevels;
        for (auto s : samples) {
			short quantized = (s / delta) * delta + delta / 2;
			quantSamples.push_back(quantized);
        }
    }

    void dumpToFile(SndfileHandle output) {
        output.write(quantSamples.data(), quantSamples.size());
    }
};

#endif

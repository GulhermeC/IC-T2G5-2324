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

    void quantBits(const std::vector<short>& samples, uint numBits) {
        for (auto s : samples) {
			short quantized = s & (0xFFFF << numBits);
            quantSamples.push_back(quantized);
        }
    }

    void dumpToFile(SndfileHandle output) {
        output.write(quantSamples.data(), quantSamples.size());
    }
};

#endif

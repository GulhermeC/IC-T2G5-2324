#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <math.h>
#include <fstream>
#include <string>
#include <fftw3.h>
#include "bitStream.h"

using namespace std;
int overflow =0;
constexpr size_t BLOCK_SIZE =16; // Buffer for reading frames
constexpr size_t SAMPLE_BITS =15; 
constexpr size_t DELTA = 200; 

std::vector<int> intToBin(int32_t n, int nbits){
	if (n < -(1 << (nbits - 1)) || n >= (1 << (nbits - 1))) {
        overflow++;
    }

	std::vector<int> binaryVector(nbits,0);

    if (n == 0) {
        binaryVector.assign(nbits,0);
    } 
	if(n<0) {
		n += (1 << nbits);
	}
	for (int i = nbits-1; i >= 0 && n > 0; --i) {
		binaryVector[i] = n % 2;
		n /= 2;
	}
	return binaryVector;
}

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
		return 1;
	}

    string outputFile = argv[argc-1];

    //Check input file
	SndfileHandle sndFile { argv[argc-2] };
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	//Check input file typemask format
	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	//Check input file submask format
	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	if((sndFile.channels())>1) {
		cerr << "Error: file is not mono\n";
		return 1;
	}


	size_t nFrames;
	vector<short> samples(BLOCK_SIZE);
	vector<double> dctCoefficients(BLOCK_SIZE);
	BitStream bts(outputFile,"w");
	int lol = 0;
	bts.writeBits(intToBin(sndFile.samplerate(),17));

	while((nFrames = sndFile.readf(samples.data(), BLOCK_SIZE))){
		lol++;
		samples.resize(nFrames);
		if(samples.size() < BLOCK_SIZE) samples.resize(BLOCK_SIZE,0);
		vector<double> samplesDouble(samples.begin(),samples.end());

		int N = samples.size();
		fftw_plan plan_dct = fftw_plan_r2r_1d(N, samplesDouble.data(), dctCoefficients.data(), FFTW_REDFT10, FFTW_ESTIMATE);
        fftw_execute(plan_dct);

		for (int i = 0; i < dctCoefficients.size(); i++){
			int quantizedCoe = round(dctCoefficients[i]/DELTA);
			bts.writeBits(intToBin(quantizedCoe,SAMPLE_BITS));
		}


	}
	cout << "overflow: " << overflow << "\n";
}
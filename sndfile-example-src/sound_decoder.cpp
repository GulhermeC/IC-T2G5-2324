#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <math.h>
#include <fstream>
#include <string>
#include <fftw3.h>
#include "bitStream.h"

using namespace std;

int binToInt(vector<int> bits){
	int sum = 0;
	if(bits[0]==1) sum = -pow(2,bits.size()-1);
	for (int i=1; i<bits.size();i++) 
		sum += bits[i] * pow(2,bits.size()-1-i);
	return sum;
}

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
		return 1;
	}

    string inputFile = argv[argc - 2];
    string outputFile = argv[argc - 1];

	BitStream bts(inputFile,"r");

	int sampleRate = binToInt(bts.readBits(17));
	int blockSize = binToInt(bts.readBits(16));
	int delta = binToInt(bts.readBits(16));
	int sampleBits = binToInt(bts.readBits(16));

    //Check input file
	SndfileHandle sndFileOut { outputFile, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, sampleRate };
    if(sndFileOut.error()){
        cerr << "Error creating the output file\n";
        return 1;
    }


	int numberBlocks = (bts.fileSize()*8)/(blockSize*sampleBits);

	vector<short> samples(numberBlocks*blockSize);

	// Vector for holding all DCT coefficients, channel by channel
	vector<vector<double>> x_dct(1, vector<double>(numberBlocks * blockSize));

	// Vector for holding DCT computations
	vector<double> x(blockSize);

	for(size_t n = 0 ; n < numberBlocks ; n++){
		for(size_t k = 0 ; k < blockSize ; k++)
			x_dct[0][n * blockSize + k] = (binToInt(bts.readBits(sampleBits))*delta);
	}
	
	fftw_plan plan_i = fftw_plan_r2r_1d(blockSize, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < numberBlocks ; n++){
		for(size_t k = 0 ; k < blockSize ; k++)
			x[k] = x_dct[0][n * blockSize + k];

		fftw_execute(plan_i);
		for(size_t k = 0 ; k < blockSize ; k++)
			samples[(n * blockSize + k)] = static_cast<short>(round(x[k]));
	}

	sndFileOut.writef(samples.data(),samples.size());
}
#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <math.h>
#include <fstream>
#include <string>
#include <fftw3.h>
#include "bitStream.h"

using namespace std;

constexpr size_t BLOCK_SIZE = 16; // Buffer for reading frames
constexpr size_t SAMPLE_BITS =15; 
constexpr size_t DELTA = 200; 

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

    //Check input file
	SndfileHandle sndFileOut { outputFile, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, sampleRate };
    if(sndFileOut.error()){
        cerr << "Error creating the output file\n";
        return 1;
    }


	int numberBlocks = (bts.fileSize()*8)/(BLOCK_SIZE*SAMPLE_BITS);

	vector<double> samplestemp(BLOCK_SIZE);
	vector<double> samples;
	vector<int> dctCoefficients(BLOCK_SIZE);

	for(int i=0; i<numberBlocks;i++){
		dctCoefficients.clear();
        for (int j = 0; j < BLOCK_SIZE; j++) {
			vector<int> quantCoesBin(bts.readBits(SAMPLE_BITS));
			dctCoefficients.push_back(binToInt(quantCoesBin)*DELTA);
        }

		vector<double> dctCoefficientsDouble(dctCoefficients.begin(),dctCoefficients.end());

		int N = BLOCK_SIZE;
		fftw_plan plan_idct = fftw_plan_r2r_1d(N, dctCoefficientsDouble.data(), samplestemp.data(), FFTW_REDFT01, FFTW_ESTIMATE);
        fftw_execute(plan_idct);

		for(int l=0;l<BLOCK_SIZE;l++){
			samples.push_back(samplestemp[l]/32);
		}

	}

	vector<short> samplesShort(samples.begin(),samples.end());

	sndFileOut.writef(samplesShort.data(),samplesShort.size());
}
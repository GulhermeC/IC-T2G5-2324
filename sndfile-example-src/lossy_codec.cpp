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

std::vector<int> intToBin(int32_t n, int nbits){
	if (n < -(pow(2,nbits-1))|| n >= (pow(2,nbits-1))) {
        overflow++;
		cout << n << "\n";
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
		cerr << "Usage: " << argv[0] << " <input file> <output file> [-s BlockSize (256)] [-d delta (500)] [-b bits per sample (7)]\n";
		return 1;
	}

    string outputFile = argv[2];
	int BlockSize = 256;
	int delta = 500;
	int sampleBits = 7;

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-s") {
			BlockSize = stoi(argv[n+1]);
			break;
		}
	
	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-d") {
			delta = stoi(argv[n+1]);
			break;
		}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-b") {
			sampleBits = stoi(argv[n+1]);
			break;
		}

    //Check input file
	SndfileHandle sndFile { argv[1] };
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
	int Mono = 1;
	if((sndFile.channels())>1) {
		Mono = 0;
	}
	size_t nChannels { static_cast<size_t>(sndFile.channels()) };
	size_t nFrames { static_cast<size_t>(sndFile.frames()) };

	vector<short> samples(nChannels * nFrames);
	sndFile.readf(samples.data(), nFrames);

	size_t nBlocks { static_cast<size_t>(ceil(static_cast<double>(nFrames) / BlockSize)) };

	// Do zero padding, if necessary
	samples.resize(nBlocks * BlockSize * nChannels);

	if(Mono==0){
		for(int i=0;i<nFrames;i++){
			short mid = ((samples[i*2] + samples[i*2+1]) / 2);
			samples[i] = mid;
		}
		nChannels = 1;
		samples.resize(nFrames);
	}
	// Vector for holding all DCT coefficients, channel by channel
	vector<vector<double>> x_dct(nChannels, vector<double>(nBlocks * BlockSize));

	// Vector for holding DCT computations
	vector<double> x(BlockSize);

	BitStream bts(outputFile,"w");
	bts.writeBits(intToBin(sndFile.samplerate(),17));
	bts.writeBits(intToBin(BlockSize,16));
	bts.writeBits(intToBin(delta,16));
	bts.writeBits(intToBin(sampleBits,16));

	fftw_plan plan_d = fftw_plan_r2r_1d(BlockSize, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++){
		for(size_t k = 0 ; k < BlockSize ; k++)
			x[k] = samples[(n * BlockSize + k) * nChannels + 0];

		fftw_execute(plan_d);
		for(size_t k = 0 ; k < BlockSize ; k++)
			x_dct[0][n * BlockSize + k] = x[k] / (BlockSize << 1);
	}

	for (int i = 0; i < x_dct[0].size(); i++){
		int quantizedCoe = round((x_dct[0][i] / delta));
		bts.writeBits(intToBin(quantizedCoe,sampleBits));
	}
	
	cout << "overflow: " << overflow << "\n";
	return 0;
}

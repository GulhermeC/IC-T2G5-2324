#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input original file>  <input certain file>\n";
		return 1;
	}

	//Check input files
	SndfileHandle OGsndFile { argv[argc-2] };
    SndfileHandle CEsndFile { argv[argc-1] };
	if(OGsndFile.error()) {
		cerr << "Error: invalid original input file\n";
		return 1;
    }
    if(CEsndFile.error()) {
		cerr << "Error: invalid certain input file\n";
		return 1;
    }

	//Check input file typemask format
	if((OGsndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: original file is not in WAV format\n";
		return 1;
	}
    if((CEsndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: certain file is not in WAV format\n";
		return 1;
	}

	//Check input file submask format
	if((OGsndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: orignal file is not in PCM_16 format\n";
		return 1;
	}
    if((CEsndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: certain file is not in PCM_16 format\n";
		return 1;
	}

	size_t nFramesO,nFramesC;
    
	vector<short> OGsamples(FRAMES_BUFFER_SIZE * OGsndFile.channels());
	vector<short> CEsamples(FRAMES_BUFFER_SIZE * CEsndFile.channels());

	vector<double> channelSums(OGsndFile.channels(), 0.0);
	vector<size_t> channelCounts(OGsndFile.channels(), 0);	

	double averageSum = 0.0;
    size_t averageCount = 0;

	vector<double> maxAbsoluteErrors(OGsndFile.channels(), 0.0);
	double maxAbsoluteError = 0.0;

	double overallAverageSNR = 0.0;

	vector<double> powerSignal(OGsndFile.channels(), 0.0);
	vector<double> powerNoise(OGsndFile.channels(), 0.0);

	double overallSignalpower = 0.0;
	double overallNoisepower = 0.0;
	
	while((nFramesO = OGsndFile.readf(OGsamples.data(), FRAMES_BUFFER_SIZE)) && (nFramesC = CEsndFile.readf(CEsamples.data(), FRAMES_BUFFER_SIZE))) {
		OGsamples.resize(nFramesO * OGsndFile.channels());
		CEsamples.resize(nFramesC * CEsndFile.channels());	
		
		for(long unsigned int i=0; i<OGsamples.size();i++) {
			size_t channelIndex =  i%OGsndFile.channels();

			//MSE
			double squaredDifference = pow(CEsamples[i]-OGsamples[i],2);
			channelSums[channelIndex] += squaredDifference;

			channelCounts[channelIndex]++;

			averageSum += squaredDifference;
			averageCount++;

			//Absolute Error
			double absoluteError = abs(CEsamples[i] - OGsamples[i]);

			if (absoluteError > maxAbsoluteErrors[channelIndex]) {
                maxAbsoluteErrors[channelIndex] = absoluteError;
            }

            if (absoluteError > maxAbsoluteError) {
                maxAbsoluteError = absoluteError;
            }

			//SNR
			powerSignal[channelIndex] += pow(OGsamples[i], 2);
			powerNoise[channelIndex] += pow(CEsamples[i] - OGsamples[i], 2);

			overallSignalpower += pow(OGsamples[i], 2);
			overallNoisepower += pow(CEsamples[i] - OGsamples[i], 2);

		}
	}	
	
	for (size_t channelIndex = 0; channelIndex < OGsndFile.channels(); channelIndex++) {
		double mse = channelSums[channelIndex] / channelCounts[channelIndex];
		cout << "MSE for Channel " << channelIndex << ": " << mse << "\n";
	}

	double averageMSE = averageSum/averageCount;
	cout << "Average MSE: " << averageMSE << "\n\n";

	for (size_t channelIndex = 0; channelIndex < OGsndFile.channels(); channelIndex++) {
        cout << "Max Absolute Error for Channel " << channelIndex << ": " << maxAbsoluteErrors[channelIndex] << "\n";
    }

	cout << "Max Absolute Error: " << maxAbsoluteError << "\n\n";

	for (size_t channelIndex = 0; channelIndex < OGsndFile.channels(); channelIndex++) {
        double channelSNR = 10 * log10(powerSignal[channelIndex] / powerNoise[channelIndex]);
        cout << "SNR for Channel " << channelIndex << ": " << channelSNR << " dB\n";
    }

	for (size_t channelIndex = 0; channelIndex < OGsndFile.channels(); channelIndex++) {
       overallAverageSNR += 10 * log10(powerSignal[channelIndex] / powerNoise[channelIndex]);
    }

	overallAverageSNR = 10 * log10(overallSignalpower / overallNoisepower);
    cout << "Average SNR across all channels: " << overallAverageSNR << " dB\n\n";

	return 0;
}

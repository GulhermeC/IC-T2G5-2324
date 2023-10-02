#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	cerr << "Channel " << argc << '\n';
	cerr << "Sample1 " << argv << '\n';
	cerr << "Sample " << *argv << '\n';

	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <channel>\n";
		return 1;
	}

	//Check input file
	SndfileHandle sndFile { argv[argc-2] };
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }
	cerr << "S " << sndFile << '\n';
	cerr << SF_FORMAT_TYPEMASK << '\n';

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

	//Check input channel
	int channel { stoi(argv[argc-1]) };
	if(channel >= sndFile.channels()) {
		cerr << "Error: invalid channel requested\n";
		return 1;
	}

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVHist hist { sndFile };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		hist.update(samples);
	}
	hist.updateBins();
	hist.dumpMid();
	hist.dumpSide();
	hist.dump(channel);
	hist.dumpBins(channel);
	return 0;
}

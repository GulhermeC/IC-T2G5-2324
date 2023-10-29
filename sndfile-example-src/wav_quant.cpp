#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"
#include <unistd.h>
#include <string.h>

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames
static const char* modes [] = {"levels","delete"};	// mode options

// Defaut input values
static unsigned int deleteBits = 8;
static unsigned int levelBits = 8;
static const char* mode = modes[0];

int main(int argc, char *argv[]) {

	const char* USAGE =  \
		"Usage: %s <input_file> <output_file> [OPTIONS]\n"
		"OPTIONS:\n"
		"	-m mode [levels|delete]     ---	quantization mode (default: levels)\n"
		"	-d delete bits              --- number of bits to zero (default: 8)\n"
		"	-l level bits               --- 2^b levels (default 8)\n";

	if(argc < 3) {
		printf(USAGE,argv[0]);
		return 1;
	}

	int opt;
	while ((opt = getopt(argc, argv, "d:l:m:h")) != -1) {

		switch (opt)
		{
			case 'd':
			{
				int option = atoi(optarg);
				if (option < 0 || option > 15) {
					fprintf(stderr, "Invalid number of delete bits \"%d\" [0:15]\n",option);
					return 1;
				}
				deleteBits = option;
				break;
			}
			case 'l':
			{
				int option = atoi(optarg);
				if (option < 1 || option > 16) {
					fprintf(stderr, "Invalid number of level bits \"%d\" [1:16]\n",option);
					return 1;
				}
				levelBits = option;
				break;
			}
			case 'm':
			{
				if ((strcmp(optarg,modes[0]) != 0 && strcmp(optarg,modes[1]) != 0)) {
					fprintf(stderr, "Bad option \"-%s\" for quantization mode [levels|delete]\n", optarg);
					return 1;
				}
				mode = optarg;
				break;
			}
			case 'h':
			{
				printf(USAGE,argv[0]);
				return 1;
			}
			default:
			{
				return 1;
			}
		}
	}

	if (strcmp(mode,modes[0]) == 0)
	{
		cout << "Mode: "  << mode << '\n';
		cout << "Levels bits: " << levelBits << '\n';
	}
	else
	{
		cout << "Mode: "  << mode << '\n';
		cout << "Delete bits: " << deleteBits << '\n';
	}

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

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVQuant quant { };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		if (strcmp(mode,modes[0]) != 0)
			quant.quantBits(samples,deleteBits);
		else
			quant.quantLevels(samples,(1 << levelBits));	// num_levels = 2^b bits
	}
	SndfileHandle sfhOut { argv[argc - 1], SFM_WRITE, sndFile.format(),
		sndFile.channels(), sndFile.samplerate() };

	quant.dumpToFile(sfhOut);
	return 0;
}

#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_effects.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames
vector<short> single_echo(SndfileHandle sndFile, int delay, float gain);
vector<short> multiple_echos(SndfileHandle sndFile, int delay, float gain);

int 
main (int argc, char *argv[])
{
    //char * fname = "sample.wav";
    
    if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <output_file> <single_echo|multiple_echos|amplitude modulation>\n";
		return 1;
	}

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

    SndfileHandle sndFileOut { argv[2], SFM_WRITE, sndFile.format(),
    sndFile.channels(), sndFile.samplerate() };
    if(sndFileOut.error()){
        cerr << "Error creating the output file\n";
        return 1;
    }

    float gain = 0.8;
    int delay = 44100;
    float freq = 1.0;
    string effect = argv[3];

    if(effect != "single_echo" && effect != "multiple_echos" && effect != "amplitude modulation")
    {
        cerr << "Error: invalid effect\n";
        return 1;
    }
    vector<short> samples_out;
  
    // Single Echo
    if(effect == "single_echo")
    {
        samples_out = single_echo(sndFile, delay, gain);
    }else{
        if(effect == "multiple_echos")
            samples_out = multiple_echos(sndFile, delay, gain);
    }

    //Save new file
    sndFileOut.writef(samples_out.data(), samples_out.size() / sndFile.channels());
    return 0;
}
/// @brief Apply Single Echo and Return Samples w/ Effect
/// @param sndFile sound file
/// @return samples w/ Echo
vector<short> single_echo(SndfileHandle sndFile, int delay, float gain)
{
    vector<short> smples_out;
    vector<short> smples(FRAMES_BUFFER_SIZE * sndFile.channels());
    size_t nFrames;
    short sample;
    smples_out.resize(0);
    while((nFrames = sndFile.readf(smples.data(), FRAMES_BUFFER_SIZE)))
    {
        smples.resize(nFrames * sndFile.channels());

        for(int n = 0; n < (int)smples.size(); n++)
        {
            if(n >= delay)
            {
                // Y(N) = X(N) + A * X(N - delay)
                sample = (smples.at(n) + gain * smples.at(n - delay)) / (1 + gain);
            }else{
                sample = smples.at(n);
            }
            smples_out.insert(smples_out.end(), sample);
        }
    }

    return smples_out;
}

/// @brief Apply Multiple Echos and Return Samples w/ Effect
/// @param sndFile sound file
/// @return samples w/ Multiple Echos
vector<short> multiple_echos(SndfileHandle sndFile, int delay, float gain)
{
    vector<short> smples_out;
    vector<short> smples(FRAMES_BUFFER_SIZE * sndFile.channels());
    size_t nFrames;
    short sample;
    smples_out.resize(0);

    while((nFrames = sndFile.readf(smples.data(), FRAMES_BUFFER_SIZE)))
    {
        smples.resize(nFrames * sndFile.channels());

        for(int n = 0; n < (int)smples.size(); n++)
        {
            if(n >= delay)
            {
                // Y(N) = X(N) + A * Y(N - delay)
                sample = (smples.at(n) + gain * smples_out.at(n - delay)) / (1 + gain);
            }else{
                sample = smples.at(n);
            }
            smples_out.insert(smples_out.end(), sample);
        }
    }

    return smples_out;
}
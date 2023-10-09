#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <math.h>


using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

// Effects
// Single echo
vector<short> single_echo(SndfileHandle sndFile, int delay, float gain);
// Multiple echo
vector<short> multiple_echos(SndfileHandle sndFile, int delay, float gain);
// Modify amplitude
vector<short> amplitude_mod(SndfileHandle sndFile, float freq);
// Reverse sound order
vector<short> reverse_snd(SndfileHandle sndFile);

int 
main (int argc, char *argv[])
{
   
    //input 
    if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <output_file> <single_echo|multiple_echos|amplitude_modulation|reverse>\n";
		return 1;
	}

    //Check input
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

    //Attempt to create output file
    SndfileHandle sndFileOut { argv[2], SFM_WRITE, sndFile.format(),
    sndFile.channels(), sndFile.samplerate() };
    if(sndFileOut.error()){
        cerr << "Error creating the output file\n";
        return 1;
    }

    //default values
    float gain = 0.8;
    int delay = 44100;
    float freq = 1.0;
    string effect = argv[3];

    //Check effect
    if(effect != "single_echo" && effect != "multiple_echos" && effect != "amplitude_modulation" && effect != "reverse")
    {
        cerr << "Error: invalid effect\n";
        return 1;
    }
    vector<short> samples_out;
  
    //Single Echo
    if(effect == "single_echo")
    {
        samples_out = single_echo(sndFile, delay, gain);
    }else{
        //Multiple Echo
        if(effect == "multiple_echos")
        {
            samples_out = multiple_echos(sndFile, delay, gain);
        }else{
            if(effect == "amplitude_modulation")
            {
                samples_out = amplitude_mod(sndFile, freq);
            }else{
                if(effect == "reverse")
                {
                    samples_out = reverse_snd(sndFile);
                }
            }
        }
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

/// @brief Change Amplitude and Return Samples w/ Effect
/// @param sndFile sound file
/// @return samples w/ changed amplitude
vector<short> amplitude_mod(SndfileHandle sndFile, float freq)
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
            // Y(N) = X(N) * cos(2*pi*(F/Fa)*N)
            sample = smples.at(n) * cos(2 * M_PI * (freq/sndFile.samplerate()) * n); 
            
            smples_out.insert(smples_out.end(), sample);
        }
    }

    return smples_out;
}

/// @brief Reverse Sample Order and Return Samples w/ Effect
/// @param sndFile sound file
/// @return reversed sample
vector<short> reverse_snd(SndfileHandle sndFile)
{
    vector<short> smples_out;
    vector<short> smples(FRAMES_BUFFER_SIZE * sndFile.channels());
    size_t nFrames;
    smples_out.resize(0);

    while((nFrames = sndFile.readf(smples.data(), FRAMES_BUFFER_SIZE)))
    {
        smples.resize(nFrames * sndFile.channels());

        for (int n = (int)smples.size() - 1; n >= 0; n--)
        {
            // Add Sample to end
            smples_out.insert(smples_out.end(), smples.at(n));
        } 
        
    }

    return smples_out;
}
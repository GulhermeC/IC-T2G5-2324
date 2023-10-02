#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_effects.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

static void 
readfile(char *fname) {

    static short buffer [FRAMES_BUFFER_SIZE];
	SndfileHandle sndFile;
    
    sndFile = SndfileHandle (fname);

    printf("Opened file '%s '\n", fname);
    //printf(" Sample rate: %d\n", sndFile.samplerate());
    //printf(" Channels: %d\n", sndFile.channels());

    sndFile.read(buffer, FRAMES_BUFFER_SIZE);
    WAVEffects effile { sndFile };
    
	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    vector<short> singleEcho;
	WAVEffects eff { sndFile };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		singleEcho = eff.singleEcho(samples);
        eff.multEcho(samples);
	}

    /* WRITE INTO WAV FILE
    while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE)))
		sfhOut.writef(samples.data(), nFrames);
	*/
	
}

int 
main (void)
{
    char * fname = "sample.wav";
    readfile (fname);
    puts("Done.\n");
    return 0;
}

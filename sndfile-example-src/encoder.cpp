#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <math.h>
#include "bitStream.h";

int 
main (int argc, char *argv[])
{
    //input 
    if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <mode>\n";
		return 1;
	}

    string fileName = argv[1];
    string fileMode = argv[2];

    BitStream(fileName, fileMode);
}

using namespace std;
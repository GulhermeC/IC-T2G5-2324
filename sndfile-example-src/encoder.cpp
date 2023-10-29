#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <math.h>
#include <fstream>
#include <string>
#include "bitStream.h"
using namespace std;

vector<int> byteToBit(char byte);
char bitToByte(vector<int> bitArr);

string fileMode;

int 
main (int argc, char *argv[])
{
    //input 
    if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <output file> <e|d|wr> [maxbit]\n";
		return 1;
	}

    int maxbit;
    string fileName = argv[1];
    string fileNameOut = argv[2];
    fileMode = argv[3];
    if(argc == 5){
            string bits = argv[4];
            maxbit = std::stoi(bits);
    }else{
        maxbit = 64;
    }
    
    BitStream bts=BitStream();

    if (fileMode=="e")
    {
        bts.ReadTxtFile(fileName, maxbit);
        bts.WriteBinFile(fileNameOut);
    }else if(fileMode=="d"){
        bts.ReadBinFile(fileName, maxbit);
        bts.WriteTxTFile(fileNameOut);
    }else if(fileMode=="wr"){
        bts.readStringFile(fileName);
        bts.WriteBinFile(fileNameOut);
    }else if(fileMode=="wd"){
        bts.ReadBinFile(fileName,maxbit);
        bts.WriteBinFile(fileNameOut);
    }

}
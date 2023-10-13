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
		cerr << "Usage: " << argv[0] << " <input file> <output file> <e|d>\n";
		return 1;
	}

    string fileName = argv[1];
    string fileNameOut=argv[2];
    fileMode = argv[3];
    BitStream bts=BitStream();

    if (fileMode=="e")
    {
        bts.ReadTxtFile(fileName);
        bts.showBits();
        bts.WriteBinFile(fileNameOut);
    }else{
        bts.ReadBinFile(fileName);
        bts.showBits();
        bts.WriteTxTFile(fileNameOut);
    }

}
//TODO CLEAN
/*
    if(fileMode=="encode")
    {
        //fazer o enconde: txt -> bin
        std::ifstream file(fileName);
        std::string line;
        std::vector<int> bits;
        //ficheiro de saida
        std::ofstream fileOut(fileNameOut,std::ios::binary);
        if (!fileOut.is_open())
        {
            std::cerr << "Erro ao criar ficheiro binário" << std::endl;
            return -1;
        }
        if(file.is_open()) {
            while (getline(file, line)){
                std::cout << line << std::endl;
                for(int i=0;i<line.size();i++)
                {
                    bits.push_back(line[i]-'0');
                    //std::cout << bits[i] << std::endl;
                }
                char convertido= bitToByte(bits);
                //gravar o byte convertido
                fileOut.write(&convertido,1);
                std::cout << std::hex << (unsigned int)convertido << std::endl;
                bits.clear();
            }
            file.close();
            fileOut.close();
        }
    }else if(fileMode=="decode")//binário para txt
    {
        std::ifstream file(fileName,std::ios::binary);
        std::string line;
        std::vector<int> bits;
        //ficheiro de saida
        std::ofstream fileOut(fileNameOut);
        if (!fileOut.is_open())
        {
            std::cerr << "Erro ao criar ficheiro texto" << std::endl;
            return -1;
        }
        char byte;
        if(file.is_open()) 
        {
            while(file.read(&byte,1)){
                //gravar no ficheiro de texto
                vector<int> bitsConvertidos=byteToBit(byte);
                for(int i=0;i<bitsConvertidos.size();i++)
                    fileOut << bitsConvertidos[i]-'0';
                fileOut << std::endl;
                
            }
        }
        file.close();
        fileOut.close();
    }

    //BitStream(fileName, fileMode);
}
vector<int> byteToBit(char byte)
    {
        vector<int> bitArr;
        for (int i = 7; i>= 0; i--)
        {
            bitArr.push_back((byte & (1 << i))? '1' : '0');
        } 

        return bitArr;

        
    }

            // Convert bit array to byte
    char bitToByte(vector<int> bitArr)
    {
        char byte = 0;

        for (int i = 0; i < 8; ++i) {
            if (bitArr[i] != 0 && bitArr[i] != 1) {
                throw std::invalid_argument("Os elementos do vetor devem ser 0 ou 1.");
            }
            byte |= (bitArr[i] << (7 - i));
         }      
        return byte;
    }
*/
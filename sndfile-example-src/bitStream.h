#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <bitset>

using namespace std;


// Bit Stream Class
class BitStream{

    private: 
        // Variables
        fstream file;
        string fName;
        string fOperation;
        int fSize;
        vector<int> bitArray;
        vector<string> lines;
        int arrayPos;
        int bitPos;
        int max = 0;
        char buffer;
    
    public:
        BitStream(){}
        BitStream(string name, string op)
        {
            fName = name;
            fOperation = op;
            bitPos = 0;
            arrayPos = 0;
            

            //Type of operation (r - read | w - write)

            if(fOperation == "r")
            {
                file.open(fName, ios::in | ios::binary);
                fSize = fileSize();
                int bit = readSingleBit();
                cout << "out bit:" << bit << "\n";
                
            }else if(fOperation == "w"){
                file.open(fName, ios::out | ios::trunc | ios::binary);
                }else{
                    cerr << "Error invalid operation\n";
            }
            
        }
        void showBits()
        {
            for(int i=0;i<bitArray.size();i++)
                std::cout << bitArray[i] << std::endl;
        }
        void ReadTxtFile(string fileName, int maxbit)
        {
            std::ifstream file(fileName);
            std::string line;

            //ficheiro de saida
            
            if(file.is_open()) {
                while (getline(file, line)){
                    if(max < maxbit){
                        std::cout << line << std::endl;
                        for(int i=0;i<line.size();i++)
                        {
                            if(max < maxbit){
                                bitArray.push_back(line[i]-'0');
                                max++;
                            }else{
                                break;
                            }
                            
                        }
                    }else{
                        max = 0;
                        break;
                        
                    }

                }
                max = 0;
                file.close();
            }
        }

        void WriteTxTFile(string fileName, int maxbit)
        {
    
            //ficheiro de saida
            std::ofstream fileOut(fileName);
            if (!fileOut.is_open())
            {
                std::cerr << "Erro ao criar ficheiro texto" << std::endl;
                return;
            }
            char byte;
            if(fileOut.is_open()) 
            {
                for(int i=0;i<bitArray.size();i++){
                    fileOut << bitArray[i];
                    if ((i+1) % 8 ==0)
                        fileOut << std::endl;
                }
                
            }
            fileOut.close();
        }

        void ReadBinFile(string fileName, int maxbit)
        {
            std::ifstream file(fileName,std::ios::binary);
            char byte;
            if(file.is_open()) 
            {
                while(file.read(&byte,1)){
                    if (max < maxbit){
                        //gravar no ficheiro de texto
                        vector<int> bitsConvertidos=byteToBit(byte);
                        for(int i=0;i<bitsConvertidos.size();i++){
                            if(max < maxbit){
                                max++;
                                bitArray.push_back(bitsConvertidos[i]-'0');
                            }else{
                                break;
                            }
                            
                        }
                    }else{
                        max = 0;
                        
                        break;
                    }
                    
                }
            }
            max = 0;
            file.close();
        }

        void WriteBinFile(string fileName, int maxbit)
        {

            //ficheiro de saida
            std::ofstream fileOut(fileName,std::ios::binary);
            if (!fileOut.is_open())
            {
                std::cerr << "Erro ao criar ficheiro binário" << std::endl;
                return;
            }
            if(fileOut.is_open()) {
                vector<int> umbyte;
                for(int i=0;i<bitArray.size();i++)
                {
                    umbyte.push_back(bitArray[i]);
                    if ((i+1) % 8 ==0){
                        
                        char convertido= bitToByte(umbyte);
                        //std::cout << std::hex << (unsigned int)convertido << std::endl;
                        fileOut.write(&convertido,1);
                        umbyte.clear();
                    }

                }
                fileOut.close();
            }
            
        }
        int fileSize()
        {
            fstream copy;
            int size;

            // Create copy
            copy.open(fName, ios::in | ios::binary);
            
            // Get file size
            copy.seekg(0, copy.end);
            size = copy.tellg();

            //cout << "size: " << size << "\n";
            return size;
        }
    //open file
    //fout.open("sample.txt", ios::app);

    // read single bit
        int readBit() {
            if (bitPos % 8 == 0) {
                file.read(&buffer,1);
                bitPos = 0;
            }
            return (buffer >> (7 - bitPos++)) & 1;
        }

    // write single bit
        void writeBit(int bit) {
            if (bitPos == 8) {
                file.write(&buffer,1);
                bitPos = 0;
                buffer = 0;
            }
            buffer = (buffer << 1) | (bit & 1);
            bitPos++;
        }

    // read N bits (0 <= N <= 64)
        std::vector<int> readBits(int numBits) {
            if (numBits < 0 || numBits > 64) {
                fprintf(stderr,"Given N (%d) must be in range [0-64]",numBits);
                return std::vector<int>(0);
            }
            std::vector<int> res (numBits);
            for(int i = 0; i < numBits; i++) {
                res[i] = readBit();
            }
            return res;
        }

    // write N bits (0 <= N <= 64)
        void writeBits(vector<int> bits) {
            size_t size = bits.size();
            if (size < 0 || size > 64) {
                fprintf(stderr,"Given N (%d) must be in range [0-64]",size);
                return;
            }
            for (uint8_t bit : bits)
                writeBit(bit);
        }

    // read string
        void readStringFile(string fileName){
            std::ifstream inputFile(fileName);
            if(!inputFile.is_open())
            {
                std::cerr << "Erro ao abrir o ficheiro" << fileName << std::endl;
                return;
            }

            std::string line;
            while(getline(inputFile, line))
            {
                lines.push_back(line);
            }
            inputFile.close();
        }
    // write string
        void writeStringFile(string fileName)
        {
            std::ofstream outputFile(fileName);
            if(!outputFile.is_open())
            {
                std::cerr << "Erro ao criar ficheiro " << fileName << std::endl;
                return; 
            }

            for (const string &line : lines)
            {
                outputFile << line << std::endl;
            }

            outputFile.close();
        }

// other classes
    
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
            byte |= (bitArr[i] << (7 - i));
         }      
        return byte;
    }
};

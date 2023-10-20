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
                
            }else if(fOperation == "w"){
                file.open(fName, ios::out | ios::trunc | ios::binary);
                }else{
                    cerr << "Error invalid operation\n";
            }
            
        }
        void showBits()
        {
            for(unsigned long int i=0;i<bitArray.size();i++)
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
                       // std::cout << line << std::endl;
                        for(unsigned long int i=0;i<line.size();i++)
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

        void WriteTxTFile(string fileName)
        {
    
            //ficheiro de saida
            std::ofstream fileOut(fileName);
            if (!fileOut.is_open())
            {
                std::cerr << "Erro ao criar ficheiro texto" << std::endl;
                return;
            }
            if(fileOut.is_open()) 
            {
                for(unsigned long int i=0;i<bitArray.size();i++){
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
                    std::cout << byte << std::endl;
                    if (max < maxbit){
                        //gravar no ficheiro de texto
                        vector<int> bitsConvertidos=byteToBit(byte);
                        for(unsigned long int i=0;i<bitsConvertidos.size();i++){
                            if(max < maxbit){
                                max++;
                                bitArray.push_back(bitsConvertidos[i]);
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

        void WriteBinFile(string fileName)
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
                //std::cout << "Bytes: " << bitArray.size() << std::endl;
                for(unsigned long int i=0;i<bitArray.size();i++)
                {
                    //std::cout << "Byte: " << i << "|" << bitArray[i] << std::endl;
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
        //Guarda uma string no bitStream em formato binário
        void stringtoBit(string txt)
        {
        
            for(unsigned long int i=0;i<txt.size();i++)
            {
                char chr=txt[i];
                vector<int> bitArr=byteToBit(chr);
                for(unsigned long int k=0;k<bitArr.size();k++)
                    bitArray.push_back(bitArr[k]);
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
            if (size > 64) {
                fprintf(stderr,"Given N (%ld) must be in range [0-64]",size);
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
            std::cout << lines.size() << " Lines read" << std::endl;
        
            for(unsigned long int i=0;i<lines.size();i++)
            {
                stringtoBit(lines[i]);
            }

        }

// other classes

    
    vector<int> byteToBit(char byte)
    {
        vector<int> bitArr;
        for (int i = 7; i>= 0; i--)
        {
            bitArr.push_back((byte & (1 << i))? 1 : 0);
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

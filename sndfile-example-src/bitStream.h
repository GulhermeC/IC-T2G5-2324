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
        int arrayPos;
        int bitPos;
    
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
        void ReadTxtFile(string fileName)
        {
            std::ifstream file(fileName);
            std::string line;

            //ficheiro de saida
            
            if(file.is_open()) {
                while (getline(file, line)){
                    std::cout << line << std::endl;
                    for(int i=0;i<line.size();i++)
                    {
                        bitArray.push_back(line[i]-'0');
                    }


                }
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

        void ReadBinFile(string fileName)
        {
            std::ifstream file(fileName,std::ios::binary);
            char byte;
            if(file.is_open()) 
            {
                while(file.read(&byte,1)){
                    //gravar no ficheiro de texto
                    vector<int> bitsConvertidos=byteToBit(byte);
                    for(int i=0;i<bitsConvertidos.size();i++)
                        bitArray.push_back(bitsConvertidos[i]-'0');
                    
                }
            }
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
        int readSingleBit() {
            
        }

    // write single bit
    

        void writeBit(int bit){
            
        }

    // read N bits (0 <= N <= 64)

    // write N bits (0 <= N <= 64)

    // read string

    // write string

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
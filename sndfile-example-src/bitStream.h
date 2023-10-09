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

        int fileSize()
        {
            fstream copy;
            int size;

            // Create copy
            copy.open(fName, ios::in | ios::binary);
            
            // Get file size
            copy.seekg(0, copy.end);
            size = copy.tellg();

            cout << "size: " << size << "\n";
            return size;
        }
    //open file
    //fout.open("sample.txt", ios::app);

    // read single bit

    // write single bit

    // read N bits (0 <= N <= 64)

    // write N bits (0 <= N <= 64)

    // read string

    // write string

    // other classes
    
};
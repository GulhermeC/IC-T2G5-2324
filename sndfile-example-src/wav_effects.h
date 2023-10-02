#ifndef WAVEFFECTS_H
#define WAVEFFECTS_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <fstream>

class WAVEffects {
    private:
        std::vector<std::map<short, size_t>> counts;
    
    public:
        WAVEffects(const SndfileHandle& sfh) {
            counts.resize(sfh.channels());
            
        }

        std::vector<short> singleEcho(const std::vector<short>& samples) {
            //std::allocator<short> position = samples.begin();
            /*
            for(short s : samples)
            {
                //std::cout << s << '\n'; 
                std::cout << s << '\n';
                // SINGLE ECHO
                // Y(N) = X(N) + AX(N)
                short simpleEcho = 0.5 * s;
                std::cout << "Echo: " <<  simpleEcho << '\n';
                //samples.insert(echo, position);
                
                
            }
            */
            for(int it = 0; it < samples.size(); it++)
            {
                short sam = samples.at(it);
                std::cout << sam << '\n';
                short prevSam = samples.at(it - 1);
                short endSam = sam + 0.5 * prevSam;
                std::cout << "Echo: " <<  endSam << '\n';
                //samples.insert(endSam, it);
            }

            return samples;
            
        }

        void multEcho(const std::vector<short>& samples) {
            // MULTIPLE ECHO
            // Y(N) = X(N) + AX(N - 1)
            for(int it = 0; it < samples.size(); it++)
            {
                short sam = samples.at(it);
                std::cout << sam << '\n';
                short prevSam = samples.at(it - 1);
                short endSam = sam + prevSam + 0.5 * prevSam;
                std::cout << "Echo: " <<  endSam << '\n';
                //samples.insert(simpleEcho, it);
                
            }
            
        }

};

#endif
#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::map<int, size_t> med;
	std::map<int, size_t> dif;

	//std::vector<float> med;
	//std::vector<short> samples;
  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		//int inicio=med.size();
		//med.resize(samples.size());
		int i=0;
		int soma=0;
		int minus=0;
		//int conta=inicio;
		for(auto s : samples){
			//std::cout << n++ % counts.size() << '\n';
			counts[n++ % counts.size()][s]++;
			//MEDIA
			//lado esquerdo
			if(i==0){
				soma = s;
				minus = s;
				i++;
			}
			else{
				//lado direito
				//std::cout << "Média de " << soma << " e de " << s << '\n';
				//std::cout << "Diferença de " << minus << " e de " << s << '\n';
				soma +=s;
				minus -= s;
				i=0;
				int media=soma/2;
				int difference = minus/2;
				//std::cout << "Méd: " << media << '\n';
				//std::cout << "Dif: " << difference << '\n';
				med[media]++;
				dif[difference]++;
				soma=0;
				minus=0;
				//std::cout << "Guardar: " <<med[media] <<  '\n';
				
			}
		}
		/*
		for(auto s : samples)
			std::cout << s << '\n';
		*/
		/*
		for(int i=0; i<counts.size();i++)
			med.push_back(counts[i]);
		*/
	}

	
	void dump(const size_t channel) const {
		//for(auto [value, counter] : counts[channel])
			//std::cout << value << '\t' << counter << '\n';
		//std::cout << "Media: " << '\n';
		for(auto [value, counter] : med){
			std::cout <<  value << '\t' << counter <<'\n';
		}
		for(auto [value, counter] : dif){
			std::cout <<  value << '\t' << counter <<'\n';
		}
		
	}
	

	/*
	void med(const size_t channels) const {
		for(int i = 1; i < channels + 1; i++){
			for(auto [value, counter] : counts[i]){
				
			}
		}
	}
	*/
};

#endif

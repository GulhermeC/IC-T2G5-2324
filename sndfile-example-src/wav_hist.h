#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <fstream>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::vector<std::map<short, size_t>> bins;
	std::map<short, size_t> average;
	std::map<short, size_t> difference;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
		bins.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };

		for(auto s : samples) {
			counts[n++ % counts.size()][s]++;
			// Only makes sense to calculate MID and SIDE for stereo
			// For every two samples (left and right) calculate mid and side
			if (counts.size() == 2 && (n % 2) == 1)
			{
				average[(samples[n-1] + samples[n]) / 2]++; 	// average[(left+right)/2]
				difference[(samples[n-1] - samples[n]) / 2]++;	// difference[(left-right)/2]
			}
		}
	}

	void updateBins(int binSize) {
		for (uint i = 0; i < counts.size(); i++) {

			int sum = 0;		// used to calculate average of values inside the considered bin
			int total = 0;		// keep track of the counters of the values in the bin
			int x = 0;			// iterate over bins

			for(auto [value, counter] : counts[i]) {
				sum += value;
				total += counter;
				x++;
				if (x == binSize) {	// create new bin
					bins[i][(sum / binSize)] = total;	// use average as index
					sum = 0;
					total = 0;
					x = 0;
				}
			}
			if (x % binSize != 0)	// handle last potential uncompleted bin
				bins[i][(sum / binSize)] = total;
		}
	}

	void dump(const size_t channel) const {
		std::ofstream count ("../histograms/count.txt");
		for(auto [value, counter] : counts[channel])
			count << value << '\t' << counter << '\n';
	}

	void dumpMid() const {
		std::ofstream mid ("../histograms/mid.txt");
		for(auto [value, counter] : average)
			mid << value << '\t' << counter << '\n';
	}

	void dumpSide() const {
		std::ofstream side ("../histograms/side.txt");
		for(auto [value, counter] : difference)
			side << value << '\t' << counter << '\n';
	}

	void dumpBins(const size_t channel) const {
		std::ofstream bin ("../histograms/bin.txt");
		for(auto [value, counter] : bins[channel])
			bin << value << '\t' << counter << '\n';
	}
};

#endif

#include "JakobsVBSProcessor.h"

struct JakobsVBSProcessor::compareSecondArgument {
	inline bool operator() (std::pair<int, float> &a, std::pair<int, float> &b) {
		return a.second > b.second;	
	}
};


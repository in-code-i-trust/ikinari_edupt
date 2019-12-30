/*
Copyright (c) 2014 hole
This software is released under the MIT License (http://kagamin.net/hole/license.txt).
A part of this software is based on smallpt (http://www.kevinbeason.com/smallpt/) and
released under the MIT License (http://kagamin.net/hole/smallpt-license.txt).
*/
#ifndef _RANDOM_H_
#define _RANDOM_H_

// #include <climits>
static const unsigned int UINT_MAX = (unsigned int)-1;

namespace edupt {

class XorShift {
	unsigned int seed_[4];
public:
	unsigned int next(void) { 
		const unsigned int t = seed_[0] ^ (seed_[0] << 11);
		seed_[0] = seed_[1]; 
		seed_[1] = seed_[2];
		seed_[2] = seed_[3];
		return seed_[3] = (seed_[3] ^ (seed_[3] >> 19)) ^ (t ^ (t >> 8)); 
	}

	double next01(void) {
		return (double)next() / UINT_MAX;
	}

	XorShift(const unsigned int initial_seed) {
		unsigned int s = initial_seed;
		for (int i = 1; i <= 4; i++){
			seed_[i-1] = s = 1812433253U * (s^(s>>30)) + i;
		}
	}
};

typedef XorShift Random;

};

#endif

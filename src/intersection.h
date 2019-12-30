/*
Copyright (c) 2014 hole
This software is released under the MIT License (http://kagamin.net/hole/license.txt).
A part of this software is based on smallpt (http://www.kevinbeason.com/smallpt/) and
released under the MIT License (http://kagamin.net/hole/smallpt-license.txt).
*/
#ifndef _INTERSECTION_H_
#define _INTERSECTION_H_

#include "vec.h"
#include "constant.h"

namespace edupt {

struct Hitpoint {
	double distance;
	Vec normal;
	Vec position;

	Hitpoint() : distance(kINF), normal(), position() {}
};

struct Intersection {
	Hitpoint hitpoint;
	int object_id;

	Intersection() : object_id(-1) {}
};

};

#endif

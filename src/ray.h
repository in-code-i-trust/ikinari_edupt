/*
Copyright (c) 2014 hole
This software is released under the MIT License (http://kagamin.net/hole/license.txt).
A part of this software is based on smallpt (http://www.kevinbeason.com/smallpt/) and
released under the MIT License (http://kagamin.net/hole/smallpt-license.txt).
*/
#ifndef _RAY_H_
#define _RAY_H_

#include "vec.h"

namespace edupt {

struct Ray {
	Vec org, dir;
	Ray(const Vec &org, const Vec &dir) : org(org), dir(dir) {}
};

};

#endif

/*
Copyright (c) 2014 hole
This software is released under the MIT License (http://kagamin.net/hole/license.txt).
A part of this software is based on smallpt (http://www.kevinbeason.com/smallpt/) and
released under the MIT License (http://kagamin.net/hole/smallpt-license.txt).
*/
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vec.h"

namespace edupt {

typedef Vec Color;

enum ReflectionType {
	REFLECTION_TYPE_DIFFUSE,	// 完全拡散面。いわゆるLambertian面。
	REFLECTION_TYPE_SPECULAR,	// 理想的な鏡面。
	REFLECTION_TYPE_REFRACTION,	// 理想的なガラス的物質。
};

const double kIor = 1.5; // 屈折率(Index of refraction)

};

#endif

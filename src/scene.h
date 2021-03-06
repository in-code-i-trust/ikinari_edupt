﻿/*
Copyright (c) 2014 hole
This software is released under the MIT License (http://kagamin.net/hole/license.txt).
A part of this software is based on smallpt (http://www.kevinbeason.com/smallpt/) and
released under the MIT License (http://kagamin.net/hole/smallpt-license.txt).
*/
#ifndef	_SCENE_H_
#define	_SCENE_H_

#include "constant.h"
#include "sphere.h"
#include "intersection.h"

namespace edupt {

// シーンとの交差判定関数
inline bool intersect_scene(const Ray &ray, Intersection *intersection) {
// レンダリングするシーンデータ
const Sphere spheres[] = {
	Sphere(1e5, Vec( 1e5+1, 40.8, 81.6), Color(),      Color(0.75, 0.25, 0.25), REFLECTION_TYPE_DIFFUSE), // 左
	Sphere(1e5, Vec(-1e5+99, 40.8, 81.6),Color(),      Color(0.25, 0.25, 0.75), REFLECTION_TYPE_DIFFUSE), // 右
	Sphere(1e5, Vec(50, 40.8, 1e5),      Color(),      Color(0.75, 0.75, 0.75), REFLECTION_TYPE_DIFFUSE), // 奥
	Sphere(1e5, Vec(50, 40.8, -1e5+250), Color(),      Color(),                 REFLECTION_TYPE_DIFFUSE), // 手前
	Sphere(1e5, Vec(50, 1e5, 81.6),      Color(),      Color(0.75, 0.75, 0.75), REFLECTION_TYPE_DIFFUSE), // 床
	Sphere(1e5, Vec(50, -1e5+81.6, 81.6),Color(),      Color(0.75, 0.75, 0.75), REFLECTION_TYPE_DIFFUSE), // 天井
	Sphere(20,Vec(65, 20, 20),           Color(),      Color(0.25, 0.75, 0.25), REFLECTION_TYPE_DIFFUSE), // 緑球
	Sphere(16.5,Vec(27, 16.5, 47),       Color(),      Color(0.99, 0.99, 0.99), REFLECTION_TYPE_SPECULAR), // 鏡
	Sphere(16.5,Vec(77, 16.5, 78),       Color(),      Color(0.99, 0.99, 0.99), REFLECTION_TYPE_REFRACTION), //ガラス
	Sphere(15.0,Vec(50.0, 90.0, 81.6),   Color(36,36,36), Color(),              REFLECTION_TYPE_DIFFUSE), //照明
};

	const double n = sizeof(spheres) / sizeof(Sphere);

	// 初期化
	intersection->hitpoint.distance = kINF;
	intersection->object_id  = -1;
	
	// 線形探索
	for (int i = 0; i < int(n); i ++) {
		Hitpoint hitpoint;
		if (spheres[i].intersect(ray, &hitpoint)) {
			if (hitpoint.distance < intersection->hitpoint.distance) {
				intersection->hitpoint = hitpoint;
				intersection->object_id = i;
			}
		}
	}

	return (intersection->object_id != -1);
}

};

#endif

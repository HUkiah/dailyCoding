#pragma once
#include<iostream>
#include<vector>
#include<math.h>
#include <regex>
#include <string>
#include <fstream>
#include<stdlib.h>

using namespace std;

struct vec3f
{
	float x, y, z;
	int index;
	inline vec3f operator =(const vec3f &v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->index = v.index;
		return *this;
	}
	inline bool operator==(vec3f &v)
	{
		if (this->x == v.x&&this->y == v.y&&this->z == v.z)
			return true;
		else
			return false;
	}
	inline bool operator!=(vec3f &v)
	{
		return !(*this == v);
	}
};

struct face
{
	vec3f verts[3];
	inline face operator=(const face &f)
	{
		for (int n = 0;n < 3;n++)
		{
			this->verts[n] = f.verts[n];
		}
		return *this;
	}
};



#include "Utils.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <Windows.h>
#include <GL/GL.h>
#include "Vector3.h"

void vec2arr(Vector3 vec, GLfloat arr[])
{
	arr[0] = vec.x;
	arr[1] = vec.y;
	arr[2] = vec.z;
}

bool IsNearlyEqual(float a, float b)
{
	const float epsilon = 1e-5f;
	return fabs(a - b) <= epsilon;
}

float clamp(float val, float lower, float upper)
{
	if (val < lower)
	{
		val = lower;
	}
	else if (val > upper)
	{
		val = upper;
	}
	return val;
}
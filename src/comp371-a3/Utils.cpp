#include "Utils.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>


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
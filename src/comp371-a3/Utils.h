#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <Windows.h>
#include <GL/GL.h>
#include "Vector3.h"

bool IsNearlyEqual(float a, float b);
float clamp(float val, float lower, float upper);
void vec2arr(Vector3 vec, GLfloat arr[]);

void drawSolidCube(GLdouble dsize);

template<typename T>
int sign(T val)
{
	return val < 0 ? -1 : 1;
}

const float toRads{ (float)M_PI / 180.0f };
const float toDegs{ 180.0f / (float)M_PI };
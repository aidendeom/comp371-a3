#include "Utils.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <Windows.h>
#include <GL/GL.h>
#include "Vector3.h"

void  drawSolidCube(GLdouble dSize)
{
	double size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size )
#	define T(a,b) glTexCoord2d(a, b)
#   define N(a,b,c) glNormal3d( a, b, c )

	/* PWO: Again, I dared to convert the code to use macros... */
	glBegin(GL_QUADS);
	N(1.0, 0.0, 0.0);
		T(0.0, 0.0); V(+, -, +);
		T(1.0, 0.0); V(+, -, -);
		T(1.0, 1.0); V(+, +, -);
		T(0.0, 1.0); V(+, +, +);

	N(0.0, 1.0, 0.0);
		T(0.0, 0.0); V(+, +, +);
		T(1.0, 0.0); V(+, +, -);
		T(1.0, 1.0); V(-, +, -);
		T(0.0, 1.0); V(-, +, +);

	N(0.0, 0.0, 1.0); 
		T(0.0, 0.0); V(+, +, +);
		T(1.0, 0.0); V(-, +, +);
		T(1.0, 1.0); V(-, -, +);
		T(0.0, 1.0); V(+, -, +);

	N(-1.0, 0.0, 0.0);
		T(0.0, 0.0); V(-, -, +);
		T(1.0, 0.0); V(-, +, +);
		T(1.0, 1.0); V(-, +, -);
		T(0.0, 1.0); V(-, -, -);

	N(0.0, -1.0, 0.0);
		T(0.0, 0.0); V(-, -, +);
		T(1.0, 0.0); V(-, -, -);
		T(1.0, 1.0); V(+, -, -);
		T(0.0, 1.0); V(+, -, +);

	N(0.0, 0.0, -1.0);
		T(0.0, 0.0); V(-, -, -);
		T(1.0, 0.0); V(-, +, -);
		T(1.0, 1.0); V(+, +, -);
		T(0.0, 1.0); V(+, -, -);

	glEnd();

#   undef V
#	undef T
#   undef N
}
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
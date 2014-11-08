#include "Vector3.h"
#include <cmath>
#include "Utils.h"

Vector3::Vector3() :
Vector3(0.0f, 0.0f, 0.0f) {}

Vector3::Vector3(float x, float y) :
Vector3(x, y, 0.0f) {}

Vector3::Vector3(float x, float y, float z) :
x(x), y(y), z(z) {}

Vector3::Vector3(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

Vector3::~Vector3()
{
}

float Vector3::magnitude() const
{
	return std::sqrtf(sqrMagnitude());
}

float Vector3::sqrMagnitude() const
{
	return x*x + y*y + z*z;
}

void Vector3::normalize()
{
	float mag = magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}

Vector3 Vector3::normalized() const
{
	float mag = magnitude();
	return Vector3(*this) / mag;
}

float Vector3::dot(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.x * rhs.x
		+ lhs.y * rhs.y
		+ lhs.z * rhs.z;
}

Vector3 Vector3::cross(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3
		(lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x);
}

bool operator==(const Vector3& lhs, const Vector3& rhs)
{
	return IsNearlyEqual(lhs.x, rhs.x)
		&& IsNearlyEqual(lhs.y, rhs.y)
		&& IsNearlyEqual(lhs.z, rhs.z);
}

bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
	return !(lhs == rhs);
}

Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vector3 operator+=(Vector3& lhs, const Vector3& rhs)
{
	lhs = lhs + rhs;
	return lhs;
}

Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vector3 operator-=(Vector3& lhs, const Vector3& rhs)
{
	lhs = lhs - rhs;
	return lhs;
}

Vector3 operator*(const Vector3& v, float f)
{
	return Vector3(v.x * f, v.y * f, v.z * f);
}

Vector3 operator*(float f, const Vector3& v)
{
	return v * f;
}

Vector3 operator/(const Vector3& v, float f)
{
	return Vector3(v.x / f, v.y / f, v.z / f);
}

Vector3 operator-(const Vector3& vec)
{
	return Vector3(-vec.x, -vec.y, -vec.z);
}

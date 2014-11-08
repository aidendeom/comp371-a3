#pragma once

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y);
	Vector3(float x, float y, float z);
	Vector3(const Vector3& other);
	~Vector3();

	float x;
	float y;
	float z;

	float magnitude() const; 
	float sqrMagnitude() const;
	void normalize();
	Vector3 normalized() const;

	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;
	static const Vector3 zero;
	static const Vector3 one;

	static float dot(const Vector3& lhs, const Vector3& rhs);
	static Vector3 cross(const Vector3& lhs, const Vector3& rhs);
	
	friend bool operator==(const Vector3& lhs, const Vector3& rhs);
	friend bool operator!=(const Vector3& lhs, const Vector3& rhs);

	friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
	friend Vector3 operator+=(Vector3& lhs, const Vector3& rhs);
	friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
	friend Vector3 operator-=(Vector3& lhs, const Vector3& rhs);
	friend Vector3 operator*(const Vector3& v, float f);
	friend Vector3 operator*(float f, const Vector3& v);
	friend Vector3 operator/(const Vector3& v, float f);
	friend Vector3 operator-(const Vector3& vec);
};


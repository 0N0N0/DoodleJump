#pragma once

template <typename T>
struct Vector3
{
	Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z;
		return(*this);
	}

	template <typename T>
	Vector3& operator*=(T s) {
		x *= s; y *= s; z *= s;
		return(*this);
	}

	template <typename T>
	Vector3& operator/=(T s) {
		return(*this *= (1.0f / s));
	}

	template <typename T>
	T& operator[](int i) {
		return(*(&x + i));
	}

	Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z;
		return(*this);
	}

	Vector3& CrossProduct(const Vector3& v) {
		return(Set(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
	}

	float DotProduct(const Vector3& v1, const Vector3& v2) {
		return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	float Length(const Vector3& v) {
		return(float(sqrt(DotProduct(v, v))));
	}

	Vector3& Normalize() {
		return (*this /= Length(*this));
	}

	T x;
	T y;
	T z;
};
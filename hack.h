#pragma once
#include "paint.h"

struct ViewMatrix {
	float Matrix[16]{};
};

//class Vector3
//{
//public:
//
//	float x, y, z;
//
//	Vector3() {};
//	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
//	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
//	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
//	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
//	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
//	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
//	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
//	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
//	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
//	float Length() const { return sqrtf(x * x + y * y + z * z); }
//	Vector3 Normalize() const { return *this * (1 / Length()); }
//	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
//};

//using Vec3 = Vector3
//using vec3 = Vector3;
//using Vec = Vector3;
//using vec = Vector3;

struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;
};

struct Vec4 {
	float x, y, z, w;
};

struct Direction {
	float yaw, pitch;
};

struct PlayerName {
	char Name[16];
};

class Hack {
private:
	Paint* paint;
public:
	uintptr_t client;
	HANDLE handle;

	uintptr_t player;
	uintptr_t entList;

	void Init();
	void Update(int windowWidth, int windowHeight);
	bool CheckValidEnt();
	bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int width, int height);

	Hack();
	Hack(Paint* paint);
};
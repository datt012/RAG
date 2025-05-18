#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
class Vector2 {
public:
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2(const float* pArg) : x(pArg[0]), y(pArg[1]) {}
	Vector2(const Vector2& vector) : x(vector.x), y(vector.y) {}
	float Length();
	Vector2& Normalize();
	Vector2 operator + (Vector2& vector);
	Vector2& operator += (Vector2& vector);
	Vector2 operator - ();
	Vector2 operator - (Vector2& vector);
	Vector2& operator -= (Vector2& vector);
	Vector2 operator * (float k);
	Vector2& operator *= (float k);
	Vector2 operator / (float k);
	Vector2& operator /= (float k);
	Vector2& operator = (Vector2 vector);
	Vector2 Modulate(Vector2& vector);
	float Dot(Vector2& vector);
	float operator [] (unsigned int idx);
	float x;
	float y;
};
class Vector3 {
public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(float* pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]) {}
	Vector3(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z) {}
	float Length();
	Vector3& Normalize();
	Vector3 operator + (Vector3& vector);
	Vector3& operator += (Vector3 vector);
	Vector3 operator - ();
	Vector3 operator - (Vector3& vector);
	Vector3& operator -= (Vector3 vector);
	Vector3 operator * (float k);
	Vector3& operator *= (float k);
	Vector3 operator / (float k);
	Vector3& operator /= (float k);
	Vector3& operator = (const Vector3& vector);
	Vector3 Modulate(Vector3& vector);
	float Dot(Vector3& vector);
	Vector3 Cross(Vector3& vector);
	float operator [] (unsigned int idx);
	float x;
	float y;
	float z;
};
class Matrix;
class Vector4 {
public:
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1.0f) {}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4(float* pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]), w(pArg[3]) {}
	Vector4(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z), w(1.0f) {}
	Vector4(const Vector3& vector, float _w) : x(vector.x), y(vector.y), z(vector.z), w(_w) {}
	Vector4(const Vector4& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}
	float Length();
	Vector4& Normalize();
	Vector4 operator + (Vector4& vector);
	Vector4& operator += (Vector4& vector);
	Vector4 operator - ();
	Vector4 operator - (Vector4& vector);
	Vector4& operator -= (Vector4& vector);
	Vector4 operator * (float k);
	Vector4& operator *= (float k);
	Vector4 operator / (float k);
	Vector4& operator /= (float k);
	Vector4& operator = (Vector4 vector);
	Vector4 Modulate(Vector4& vector);
	float Dot(Vector4& vector);
	Vector4 operator * (Matrix m);
	float operator [] (unsigned int idx);
	float x;
	float y;
	float z;
	float w;
};
class Matrix {
public:
	Matrix() {}
	Matrix(float val);
	Matrix(const Matrix& mat);
	Matrix& SetZero();
	Matrix& SetIdentity();
	Matrix& SetRotationX(float angle);
	Matrix& SetRotationY(float angle);
	Matrix& SetRotationZ(float angle);
	Matrix& SetRotationAngleAxis(float angle, float x, float y, float z);
	Matrix& SetScale(float scale);
	Matrix& SetScale(float scaleX, float scaleY, float scaleZ);
	Matrix& SetScale(float* pScale);
	Matrix& SetScale(Vector3& scaleVec);
	Matrix& SetTranslation(float x, float y, float z);
	Matrix& SetTranslation(float* pTrans);
	Matrix& SetTranslation(Vector3& vec);
	Matrix& SetOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane);
	Matrix& SetPerspective(float fovY, float aspect, float nearPlane, float farPlane);
	Matrix Transpose();
	Matrix operator + (Matrix& mat);
	Matrix& operator += (Matrix& mat);
	Matrix operator - (Matrix& mat);
	Matrix& operator -= (Matrix& mat);
	Matrix operator * (Matrix mat);
	Matrix operator * (float k);
	Matrix& operator *= (float k);
	Vector4 operator * (Vector4& vec);
	Matrix& operator = (Matrix mat);
	float m[4][4] = { 0 };
};
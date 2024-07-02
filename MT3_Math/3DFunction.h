#pragma once
#include <cmath>
#include <assert.h>
#include <Vector2.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include "Novice.h"
#include <corecrt_math_defines.h>

#pragma region 结构体

/// <summary>
/// 定义 球的结构体
/// </summary>
typedef struct {
	Vector3 center;
	float radius;
}Sphere;

/// <summary>
/// 线
/// </summary>
typedef struct {
	Vector3 origin;		//起点
	Vector3 diff;		//到终点的差分向量
}Line;

/// <summary>
/// 半直线
/// </summary>
typedef struct {
	Vector3 origin;		//起点
	Vector3 diff;		//到终点的差分向量
}Ray;

/// <summary>
/// 线分
/// </summary>
typedef struct {
	Vector3 origin;		//起点
	Vector3 diff;		//到终点的差分向量
}Segment;

/// <summary>
/// 平面结构体
/// </summary>
typedef struct {
	Vector3 normal;//法线
	float distance;//距离
}Plane;

/// <summary>
/// 三角形结构体
/// </summary>
typedef struct {
	Vector3 vertices[3];
}Triangle;

/// <summary>
/// AABB结构体
/// </summary>
typedef struct {
	Vector3 min;
	Vector3 max;
}AABB;

/// <summary>
/// 弹簧
/// </summary>
typedef struct
{
	Vector3 anchor;				//固定端的位置
	float naturalLength;		//自然长度
	float stiffness;			//刚性系数
	float dampingCoefficirent;	//衰减系数
}Spring;

/// <summary>
/// 球
/// </summary>
typedef struct
{
	Vector3 position;		//位置
	Vector3 velocity;		//速度
	Vector3 acceleration;	//加速度
	float mass;				//质量
	float radius;			//半径
	unsigned int color;		//颜色
}Ball;

/// <summary>
/// 单摆（振子）
/// </summary>
typedef struct {
	Vector3 anchor;             // 固定端
	float length;               // 绳长
	float angle;                // 角度
	float angularVelocity;      // 角速度
	float angularAcceleration;  // 角加速度
}Pendulum;

/// <summary>
/// 圆锥振子
/// </summary>
typedef struct {
	Vector3 anchor;             // 固定点
	float length;               // 绳长
	float halfApexAngle;        // 圆锥顶角的一半
	float angle;                // 角度
	float angularVelocity;      // 角速度
}ConicalPendulum;

/// <summary>
/// 胶囊体
/// </summary>
typedef struct {
	Segment segment;
	float radius;
}Capsule;


#pragma endregion

#pragma region 数学公式

/// <summary>
/// 加算
/// </summary>
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result = { 0,0,0 };
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

/// <summary>
/// 減算
/// </summary>
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result = { 0,0,0 };
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

/// <summary>
/// スカラー倍
/// </summary>
Vector3 Multiply(float Scaler, const Vector3& v) {
	Vector3 result = { 0,0,0 };
	result.x = Scaler * v.x;
	result.y = Scaler * v.y;
	result.z = Scaler * v.z;
	return result;
}

/// <summary>
/// 内積
/// </summary>
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// <summary>
/// 長さ
/// </summary>
float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

/// <summary>
/// 计算平方长度
/// </summary>
/// <param name="v">目标向量</param>
/// <returns></returns>
float LengthSquared(const Vector3& v) {
	return Dot(v, v);
}
/// <summary>
/// 正規化
/// </summary>
Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	if (length != 0)
	{
		Vector3 result = { 0,0,0 };
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
		return result;
	}
	else
	{
		Vector3 result = { 0,0,0 };
		return result;
	}
}

/// <summary>
/// 行列加算
/// </summary>
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

/// <summary>
/// 行列減算
/// </summary>
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

/// <summary>
/// 行列乗算
/// </summary>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

/// <summary>
/// 逆行列
/// </summary>
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result = {};
	float det;
	int i, j;

	result.m[0][0] = m.m[1][1] * m.m[2][2] * m.m[3][3] -
		m.m[1][1] * m.m[2][3] * m.m[3][2] -
		m.m[2][1] * m.m[1][2] * m.m[3][3] +
		m.m[2][1] * m.m[1][3] * m.m[3][2] +
		m.m[3][1] * m.m[1][2] * m.m[2][3] -
		m.m[3][1] * m.m[1][3] * m.m[2][2];

	result.m[1][0] = -m.m[1][0] * m.m[2][2] * m.m[3][3] +
		m.m[1][0] * m.m[2][3] * m.m[3][2] +
		m.m[2][0] * m.m[1][2] * m.m[3][3] -
		m.m[2][0] * m.m[1][3] * m.m[3][2] -
		m.m[3][0] * m.m[1][2] * m.m[2][3] +
		m.m[3][0] * m.m[1][3] * m.m[2][2];

	result.m[2][0] = m.m[1][0] * m.m[2][1] * m.m[3][3] -
		m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[2][0] * m.m[1][1] * m.m[3][3] +
		m.m[2][0] * m.m[1][3] * m.m[3][1] +
		m.m[3][0] * m.m[1][1] * m.m[2][3] -
		m.m[3][0] * m.m[1][3] * m.m[2][1];

	result.m[3][0] = -m.m[1][0] * m.m[2][1] * m.m[3][2] +
		m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[2][0] * m.m[1][1] * m.m[3][2] -
		m.m[2][0] * m.m[1][2] * m.m[3][1] -
		m.m[3][0] * m.m[1][1] * m.m[2][2] +
		m.m[3][0] * m.m[1][2] * m.m[2][1];

	result.m[0][1] = -m.m[0][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][1] * m.m[2][3] * m.m[3][2] +
		m.m[2][1] * m.m[0][2] * m.m[3][3] -
		m.m[2][1] * m.m[0][3] * m.m[3][2] -
		m.m[3][1] * m.m[0][2] * m.m[2][3] +
		m.m[3][1] * m.m[0][3] * m.m[2][2];

	result.m[1][1] = m.m[0][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][0] * m.m[2][3] * m.m[3][2] -
		m.m[2][0] * m.m[0][2] * m.m[3][3] +
		m.m[2][0] * m.m[0][3] * m.m[3][2] +
		m.m[3][0] * m.m[0][2] * m.m[2][3] -
		m.m[3][0] * m.m[0][3] * m.m[2][2];

	result.m[2][1] = -m.m[0][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][0] * m.m[2][3] * m.m[3][1] +
		m.m[2][0] * m.m[0][1] * m.m[3][3] -
		m.m[2][0] * m.m[0][3] * m.m[3][1] -
		m.m[3][0] * m.m[0][1] * m.m[2][3] +
		m.m[3][0] * m.m[0][3] * m.m[2][1];

	result.m[3][1] = m.m[0][0] * m.m[2][1] * m.m[3][2] -
		m.m[0][0] * m.m[2][2] * m.m[3][1] -
		m.m[2][0] * m.m[0][1] * m.m[3][2] +
		m.m[2][0] * m.m[0][2] * m.m[3][1] +
		m.m[3][0] * m.m[0][1] * m.m[2][2] -
		m.m[3][0] * m.m[0][2] * m.m[2][1];

	result.m[0][2] = m.m[0][1] * m.m[1][2] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[3][2] -
		m.m[1][1] * m.m[0][2] * m.m[3][3] +
		m.m[1][1] * m.m[0][3] * m.m[3][2] +
		m.m[3][1] * m.m[0][2] * m.m[1][3] -
		m.m[3][1] * m.m[0][3] * m.m[1][2];

	result.m[1][2] = -m.m[0][0] * m.m[1][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][3] * m.m[3][2] +
		m.m[1][0] * m.m[0][2] * m.m[3][3] -
		m.m[1][0] * m.m[0][3] * m.m[3][2] -
		m.m[3][0] * m.m[0][2] * m.m[1][3] +
		m.m[3][0] * m.m[0][3] * m.m[1][2];

	result.m[2][2] = m.m[0][0] * m.m[1][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][3] * m.m[3][1] -
		m.m[1][0] * m.m[0][1] * m.m[3][3] +
		m.m[1][0] * m.m[0][3] * m.m[3][1] +
		m.m[3][0] * m.m[0][1] * m.m[1][3] -
		m.m[3][0] * m.m[0][3] * m.m[1][1];

	result.m[3][2] = -m.m[0][0] * m.m[1][1] * m.m[3][2] +
		m.m[0][0] * m.m[1][2] * m.m[3][1] +
		m.m[1][0] * m.m[0][1] * m.m[3][2] -
		m.m[1][0] * m.m[0][2] * m.m[3][1] -
		m.m[3][0] * m.m[0][1] * m.m[1][2] +
		m.m[3][0] * m.m[0][2] * m.m[1][1];

	result.m[0][3] = -m.m[0][1] * m.m[1][2] * m.m[2][3] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] +
		m.m[1][1] * m.m[0][2] * m.m[2][3] -
		m.m[1][1] * m.m[0][3] * m.m[2][2] -
		m.m[2][1] * m.m[0][2] * m.m[1][3] +
		m.m[2][1] * m.m[0][3] * m.m[1][2];

	result.m[1][3] = m.m[0][0] * m.m[1][2] * m.m[2][3] -
		m.m[0][0] * m.m[1][3] * m.m[2][2] -
		m.m[1][0] * m.m[0][2] * m.m[2][3] +
		m.m[1][0] * m.m[0][3] * m.m[2][2] +
		m.m[2][0] * m.m[0][2] * m.m[1][3] -
		m.m[2][0] * m.m[0][3] * m.m[1][2];

	result.m[2][3] = -m.m[0][0] * m.m[1][1] * m.m[2][3] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] +
		m.m[1][0] * m.m[0][1] * m.m[2][3] -
		m.m[1][0] * m.m[0][3] * m.m[2][1] -
		m.m[2][0] * m.m[0][1] * m.m[1][3] +
		m.m[2][0] * m.m[0][3] * m.m[1][1];

	result.m[3][3] = m.m[0][0] * m.m[1][1] * m.m[2][2] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] -
		m.m[1][0] * m.m[0][1] * m.m[2][2] +
		m.m[1][0] * m.m[0][2] * m.m[2][1] +
		m.m[2][0] * m.m[0][1] * m.m[1][2] -
		m.m[2][0] * m.m[0][2] * m.m[1][1];

	det = m.m[0][0] * result.m[0][0] + m.m[0][1] * result.m[1][0] + m.m[0][2] * result.m[2][0] + m.m[0][3] * result.m[3][0];

	if (det == 0)
		return m;

	det = 1.0f / det;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			result.m[i][j] *= det;
		}
	}

	return result;

}

/// <summary>
/// 転置行列
/// </summary>
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

/// <summary>
/// 単位行列の作成
/// </summary>
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++)
	{
		result.m[i][i] = 1.0f;
	}
	return result;
}


Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 result = {};

	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 result = {};

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result = {};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = std::sin(radian) * -1.0f;
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {};


	result.m[0][0] = std::cos(radian);
	result.m[0][2] = std::sin(radian) * -1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = std::sin(radian) * -1.0f;
	result.m[1][1] = std::cos(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);

	return Multiply(Multiply(MakeScaleMatrix(scale), rotateMatrix), MakeTranslateMatrix(translate));

}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};

	result.m[0][0] = (1.0f / aspectRatio) * std::cos(fovY / 2.0f) / std::sin(fovY / 2.0f);
	result.m[1][1] = std::cos(fovY / 2.0f) / std::sin(fovY / 2.0f);
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1;
	result.m[3][2] = nearClip * farClip * -1.0f / (farClip - nearClip);

	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {};

	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = height / 2.0f * -1;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;

	return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

/// <summary>
/// 线性插值函数
/// </summary>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Add(Multiply((1.0f - t), v1), Multiply(t, v2));
}

#pragma endregion

#pragma region 运算符重载

// 向量二项运算符重载
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }
inline Vector3 operator*(float Scaler, const Vector3& v) { return Multiply(Scaler, v); }
inline Vector3 operator*(const Vector3& v, float Scaler) { return Scaler * v; }
inline Vector3 operator/(const Vector3& v, float Scaler) { return (1.0f / Scaler) * v; }

// 向量单项运算符重载
inline Vector3 operator+(const Vector3& v) { return v; }
inline Vector3 operator-(const Vector3& v) { return Vector3(-v.x, -v.y, -v.z); }

// 矩阵二项运算符重载
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return Add(m1, m2); }
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return Subtract(m1, m2); }
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Multiply(m1, m2); }

#pragma endregion

#pragma region 矩阵表示

/// <summary>
/// 表示
/// </summary>
static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

/// <summary>
/// 表示
/// </summary>
static const int kRowHeight = 20;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y - kRowHeight, "%s", label);
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}
#pragma endregion

#pragma region 正射影相关

/// <summary>
/// V1 在 V2上的正射影
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Project(const Vector3& v1, const Vector3& v2) {

	float dot = Dot(v1, v2);
	float len = LengthSquared(v2);
	float t = dot / len;
	return Multiply(t, v2);
}

/// <summary>
/// 点和线的最近距离的点
/// </summary>
/// <param name="point">点</param>
/// <param name="segment">目标线分</param>
/// <returns>最近的距离的点</returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {

	Vector3 o = segment.origin;
	Vector3 p = point;
	Vector3 a = Subtract(p, o);
	Vector3 b = segment.diff;


	Vector3 cp = Project(a, b);

	return Add(o, cp);
}

#pragma endregion

#pragma region 画各种

/// <summary>
/// 画球
/// </summary>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Matrix4x4 worldViewProjectionMatrix = viewProjectionMatrix;
	Matrix4x4 viewPortMatrix = viewportMatrix;

	const uint32_t kSubdivision = 10;	//分割数
	const float kLonEvery = 2.0f * float(M_PI) / float(kSubdivision);		//経度一つ分の角度
	const float kLatEvery = float(M_PI) / float(kSubdivision);		//緯度一つ分の角度

	Vector3 a;
	Vector3 b;
	Vector3 c;
	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * float(latIndex);		//現在の緯度
		//経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = kLonEvery * float(lonIndex);		//現在の経度
			//球面上の座標を求める
			a.x = sphere.center.x + sphere.radius * cos(lat) * cos(lon);
			a.y = sphere.center.y + sphere.radius * sin(lat);
			a.z = sphere.center.z + sphere.radius * cos(lat) * sin(lon);

			b.x = sphere.center.x + sphere.radius * cos(lat + kLatEvery) * cos(lon);
			b.y = sphere.center.y + sphere.radius * sin(lat + kLatEvery);
			b.z = sphere.center.z + sphere.radius * cos(lat + kLatEvery) * sin(lon);

			c.x = sphere.center.x + sphere.radius * cos(lat) * cos(lon + kLonEvery);
			c.y = sphere.center.y + sphere.radius * sin(lat);
			c.z = sphere.center.z + sphere.radius * cos(lat) * sin(lon + kLonEvery);
			//画面上の座標を求める
			Vector3 ndcA = Transform(a, worldViewProjectionMatrix);
			Vector3 ndcB = Transform(b, worldViewProjectionMatrix);
			Vector3 ndcC = Transform(c, worldViewProjectionMatrix);
			Vector3 screenA = Transform(ndcA, viewPortMatrix);
			Vector3 screenB = Transform(ndcB, viewPortMatrix);
			Vector3 screenC = Transform(ndcC, viewPortMatrix);

			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenB.x, (int)screenB.y, color);
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenC.x, (int)screenC.y, color);
		}
	}

}

/// <summary>
/// 画网格
/// </summary>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const uint32_t kSubdivision = 10;//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//一つ分の長さ

	Matrix4x4 worldViewProjectionMatrix = viewProjectionMatrix;
	Matrix4x4 viewPortMatrix = viewportMatrix;

	Vector3 startPosHorizontal[kSubdivision + 1];
	Vector3 endPosHorizontal[kSubdivision + 1];
	Vector3 startPosScreenHorizontal[kSubdivision + 1];
	Vector3 endPosScreenHorizontal[kSubdivision + 1];

	Vector3 startPosVertical[kSubdivision + 1];
	Vector3 endPosVertical[kSubdivision + 1];
	Vector3 startPosScreenVertical[kSubdivision + 1];
	Vector3 endPosScreenVertical[kSubdivision + 1];
	//从深处到面前按顺序画线
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//使用上面的信息，求出在world坐标里的起点和终点坐标
		startPosHorizontal[xIndex].x = kGridHalfWidth;
		endPosHorizontal[xIndex].x = -kGridHalfWidth;

		startPosHorizontal[xIndex].y = 0.0f;
		endPosHorizontal[xIndex].y = 0.0f;

		startPosHorizontal[xIndex].z = kGridHalfWidth - xIndex * kGridEvery;
		endPosHorizontal[xIndex].z = kGridHalfWidth - xIndex * kGridEvery;
		//变换成screen坐标
		Vector3 ndcStartPosHorizontal = Transform(startPosHorizontal[xIndex], worldViewProjectionMatrix);
		Vector3 ndcEndPosHorizontal = Transform(endPosHorizontal[xIndex], worldViewProjectionMatrix);

		startPosScreenHorizontal[xIndex] = Transform(ndcStartPosHorizontal, viewPortMatrix);
		endPosScreenHorizontal[xIndex] = Transform(ndcEndPosHorizontal, viewPortMatrix);
		////使用变换后的坐标画线

		if (xIndex == 5) {
			Novice::DrawLine((int)startPosScreenHorizontal[xIndex].x, (int)startPosScreenHorizontal[xIndex].y, (int)endPosScreenHorizontal[xIndex].x, (int)endPosScreenHorizontal[xIndex].y, BLACK);
		}
		else {
			Novice::DrawLine((int)startPosScreenHorizontal[xIndex].x, (int)startPosScreenHorizontal[xIndex].y, (int)endPosScreenHorizontal[xIndex].x, (int)endPosScreenHorizontal[xIndex].y, 0xAAAAAAFF);
		}

	}
	//从左到右
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		startPosVertical[zIndex].x = kGridHalfWidth - zIndex * kGridEvery;
		endPosVertical[zIndex].x = kGridHalfWidth - zIndex * kGridEvery;

		startPosVertical[zIndex].y = 0.0f;
		endPosVertical[zIndex].y = 0.0f;

		startPosVertical[zIndex].z = kGridHalfWidth;
		endPosVertical[zIndex].z = -kGridHalfWidth;

		Vector3 ndcStartPosVertical = Transform(startPosVertical[zIndex], worldViewProjectionMatrix);
		Vector3 ndcEndPosVertical = Transform(endPosVertical[zIndex], worldViewProjectionMatrix);

		startPosScreenVertical[zIndex] = Transform(ndcStartPosVertical, viewPortMatrix);
		endPosScreenVertical[zIndex] = Transform(ndcEndPosVertical, viewPortMatrix);

		if (zIndex == 5) {
			Novice::DrawLine((int)startPosScreenVertical[zIndex].x, (int)startPosScreenVertical[zIndex].y, (int)endPosScreenVertical[zIndex].x, (int)endPosScreenVertical[zIndex].y, BLACK);
		}
		else {
			Novice::DrawLine((int)startPosScreenVertical[zIndex].x, (int)startPosScreenVertical[zIndex].y, (int)endPosScreenVertical[zIndex].x, (int)endPosScreenVertical[zIndex].y, 0xAAAAAAFF);
		}

	}
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

/// <summary>
/// 画平面
/// </summary>
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendicular[4];
	perpendicular[0] = Normalize(Perpendicular(plane.normal));
	perpendicular[1] = { -perpendicular[0].x,-perpendicular[0].y,-perpendicular[0].z };
	perpendicular[2] = Cross(plane.normal, perpendicular[0]);
	perpendicular[3] = { -perpendicular[2].x,-perpendicular[2].y,-perpendicular[2].z };

	Vector3 points[4];
	for (uint32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendicular[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[2].x, (int)points[2].y, (int)points[1].x, (int)points[1].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[3].x, (int)points[3].y, (int)points[0].x, (int)points[0].y, color);

}

/// <summary>
/// 画三角形
/// </summary>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {
	Vector3 screenVertices[3];
	for (uint32_t i = 0; i < 3; ++i) {
		Vector3 ndcVertex = Transform(triangle.vertices[i], viewProjectionMatrix);
		screenVertices[i] = Transform(ndcVertex, viewPortMatrix);
	}
	Novice::DrawTriangle((int)(screenVertices[0].x), (int)(screenVertices[0].y), (int)(screenVertices[1].x), (int)(screenVertices[1].y), (int)(screenVertices[2].x), (int)(screenVertices[2].y), color, kFillModeWireFrame);
}

/// <summary>
/// 画AABB
/// </summary>
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {
	Vector3 points[8];
	points[0] = Vector3(aabb.min.x, aabb.min.y, aabb.min.z);
	points[1] = Vector3(aabb.max.x, aabb.min.y, aabb.min.z);
	points[2] = Vector3(aabb.max.x, aabb.min.y, aabb.max.z);
	points[3] = Vector3(aabb.min.x, aabb.min.y, aabb.max.z);

	points[4] = Vector3(aabb.min.x, aabb.max.y, aabb.min.z);
	points[5] = Vector3(aabb.max.x, aabb.max.y, aabb.min.z);
	points[6] = Vector3(aabb.max.x, aabb.max.y, aabb.max.z);
	points[7] = Vector3(aabb.min.x, aabb.max.y, aabb.max.z);

	Vector3 screenPoints[8];
	for (uint32_t index = 0; index < 8; ++index) {
		Vector3 ndcVertex = Transform(points[index], viewProjectionMatrix);
		screenPoints[index] = Transform(ndcVertex, viewPortMatrix);
	}

	Novice::DrawLine((int)screenPoints[0].x, (int)screenPoints[0].y, (int)screenPoints[1].x, (int)screenPoints[1].y, color);
	Novice::DrawLine((int)screenPoints[1].x, (int)screenPoints[1].y, (int)screenPoints[2].x, (int)screenPoints[2].y, color);
	Novice::DrawLine((int)screenPoints[2].x, (int)screenPoints[2].y, (int)screenPoints[3].x, (int)screenPoints[3].y, color);
	Novice::DrawLine((int)screenPoints[3].x, (int)screenPoints[3].y, (int)screenPoints[0].x, (int)screenPoints[0].y, color);

	Novice::DrawLine((int)screenPoints[4].x, (int)screenPoints[4].y, (int)screenPoints[5].x, (int)screenPoints[5].y, color);
	Novice::DrawLine((int)screenPoints[5].x, (int)screenPoints[5].y, (int)screenPoints[6].x, (int)screenPoints[6].y, color);
	Novice::DrawLine((int)screenPoints[6].x, (int)screenPoints[6].y, (int)screenPoints[7].x, (int)screenPoints[7].y, color);
	Novice::DrawLine((int)screenPoints[7].x, (int)screenPoints[7].y, (int)screenPoints[4].x, (int)screenPoints[4].y, color);

	Novice::DrawLine((int)screenPoints[0].x, (int)screenPoints[0].y, (int)screenPoints[4].x, (int)screenPoints[4].y, color);
	Novice::DrawLine((int)screenPoints[1].x, (int)screenPoints[1].y, (int)screenPoints[5].x, (int)screenPoints[5].y, color);
	Novice::DrawLine((int)screenPoints[2].x, (int)screenPoints[2].y, (int)screenPoints[6].x, (int)screenPoints[6].y, color);
	Novice::DrawLine((int)screenPoints[3].x, (int)screenPoints[3].y, (int)screenPoints[7].x, (int)screenPoints[7].y, color);
}

/// <summary>
/// 画3次贝塞尔曲线
/// </summary>
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	const int numSegments = 100;
	Vector3 previousPoint = controlPoint0;

	for (int i = 1; i <= numSegments; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(numSegments);
		Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, t);
		Vector3 p1p2 = Lerp(controlPoint1, controlPoint2, t);
		Vector3 currentPoint = Lerp(p0p1, p1p2, t);

		Vector3 start = Transform(Transform(previousPoint, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(currentPoint, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, color);

		previousPoint = currentPoint;
	}
}

#pragma endregion

#pragma region Collision 判定
/// <summary>
/// 冲突判定 球与球
/// </summary>
/// <param name="sphere1">球1</param>
/// <param name="sphere2">球2</param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2) {
	float distance = Length(Subtract(sphere1.center, sphere2.center));
	if (distance > sphere1.radius + sphere2.radius) {
		return false;
	}
	return true;
}



/// <summary>
/// 冲突判定 球和平面
/// </summary>
/// <param name="sphere">球</param>
/// <param name="plane">平面</param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const Plane& plane) {

	Vector3 n = plane.normal;
	Vector3 c = sphere.center;

	float k = Dot(n, c) - plane.distance;

	float distance = fabs(k);

	if (distance > sphere.radius) {
		return false;
	}

	return true;

}


/// <summary>
/// 冲突判定 线和平面
/// </summary>
/// <param name="segment"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Segment& segment, const Plane& plane) {
	float dot = Dot(plane.normal, segment.diff);

	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	return true;

}

/// <summary>
/// 冲突判定 三角形和线
/// </summary>
/// <param name="triangle"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment) {

	//计算三角形所在平面
	Vector3 ab = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 ac = Subtract(triangle.vertices[2], triangle.vertices[0]);
	Vector3 normal = Cross(ab, ac);
	float distance = Dot(normal, triangle.vertices[0]);


	float dot = Dot(normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}

	//计算t
	float t = (distance - Dot(normal, segment.origin)) / dot;
	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	//计算交点在不在三角形内
	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));
	Vector3 crossAB = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
	Vector3 crossBC = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
	Vector3 crossCA = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));
	if (Dot(crossAB, normal) >= 0 && Dot(crossBC, normal) >= 0 && Dot(crossCA, normal) >= 0) {
		return true;
	}
	return false;
}

/// <summary>
/// 检测AABB是否合理
/// </summary>
/// <param name="aabb"></param>
void CorrectAABB(AABB& aabb) {
	aabb.max.x = std::max(aabb.max.x, aabb.min.x);
	aabb.min.x = std::min(aabb.min.x, aabb.max.x);

	aabb.max.y = std::max(aabb.max.y, aabb.min.y);
	aabb.min.y = std::min(aabb.min.y, aabb.max.y);

	aabb.max.z = std::max(aabb.max.z, aabb.min.z);
	aabb.min.z = std::min(aabb.min.z, aabb.max.z);
}

/// <summary>
/// 冲突判定 AABB与AABB
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	return false;
}

/// <summary>
/// 冲突判定 aabb和球
/// </summary>
/// <param name="aabb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	//最近接点
	Vector3 clossestPoint
	{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};
	//最近接点和球心距离
	float distance = Length(Subtract(clossestPoint, sphere.center));

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

/// <summary>
/// 冲突判定 AABBを线
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment)
{
	float tNearX = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tFarX = (aabb.max.x - segment.origin.x) / segment.diff.x;
	if (tNearX > tFarX) std::swap(tNearX, tFarX);

	float tNearY = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tFarY = (aabb.max.y - segment.origin.y) / segment.diff.y;
	if (tNearY > tFarY) std::swap(tNearY, tFarY);

	float tNearZ = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tFarZ = (aabb.max.z - segment.origin.z) / segment.diff.z;
	if (tNearZ > tFarZ) std::swap(tNearZ, tFarZ);


	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);


	if (tmin <= tmax && tmax >= 0.0f && tmin <= 1.0f) {
		return true;
	}
	return false;
}

#pragma endregion

#pragma region Camera Controller
// マウスでカメラを移動
Vector2 preMousePos{}; // 前１フレームの位置
// マウスでカメラを制御する
void MouseCamera(Vector3* cameraPos, Vector3* cameraRotate, char key[]) {
	float moveSpeed = 0.1f;     // キーボードで移動のスピード
	float wheelSpeed = 0.3f;    // マウスのホイールスクロールのスピード
	float rotationSpeed = 0.4f; // マウスの右キーで回るスピード
	float dragSpeed = 0.5f;     // マウスの中キーで移動のスピード

	Vector3 front{}, right{}, up{}, move{}; // カメラの前・横・上の向きと総合の移動ベクトル
	bool isMouseMove = false;               // マウスで移動
	//  カメラの前方向を計算
	front.x = sinf(cameraRotate->y) * cosf(cameraRotate->x);
	front.y = -sinf(cameraRotate->x);
	front.z = cosf(cameraRotate->y) * cosf(cameraRotate->x);
	front = Normalize(front);
	// カメラの横方向を計算
	Vector3 worldUp{ 0, 1, 0 };
	right = Cross(front, worldUp);
	right = Normalize(right);
	// カメラの上方向を計算
	up = Cross(right, front);
	up = Normalize(up);

	if (Novice::IsPressMouse(1)) {
		// キーボードで移動
		if (key[DIK_W]) {
			move = Add(move, front);
		}
		else if (key[DIK_S]) {
			move = Add(move, Multiply(-1, front));
		}
		if (key[DIK_D]) {
			move = Add(move, Multiply(-1, right));
		}
		else if (key[DIK_A]) {
			move = Add(move, right);
		}
	}
	else {
		// マウスのホイールスクロール
		isMouseMove = true;
		move = Multiply(float(Novice::GetWheel()) * wheelSpeed * 0.01f, front);
	}

	// カメラをマウスで回転
	int mousePosX, mousePosY;
	Novice::GetMousePosition(&mousePosX, &mousePosY);
	Vector2 currentMousePos{};
	if (Novice::IsPressMouse(1)) {
		// マウスの右キー
		currentMousePos = { float(mousePosX), float(mousePosY) };
		cameraRotate->x += (currentMousePos.y - preMousePos.y) * rotationSpeed * 0.01f;
		cameraRotate->y += (currentMousePos.x - preMousePos.x) * rotationSpeed * 0.01f;
		preMousePos = { float(mousePosX), float(mousePosY) };
	}
	else if (Novice::IsPressMouse(2)) {
		// マウスの中キー
		isMouseMove = true;
		currentMousePos = { float(mousePosX), float(mousePosY) };
		Vector3 mouseVector = { currentMousePos.x - preMousePos.x, currentMousePos.y - preMousePos.y, 0 };
		if (abs(mouseVector.x) > 1 || abs(mouseVector.y) > 1) {
			move = Add(move, Multiply(mouseVector.x * dragSpeed * 0.01f, right));
			move = Add(move, Multiply(mouseVector.y * dragSpeed * 0.01f, up));
		}
		preMousePos = { float(mousePosX), float(mousePosY) };
	}
	else {
		preMousePos = { float(mousePosX), float(mousePosY) };
	}

	// 正規化、速度は同じにするために
	if (!isMouseMove) {
		if (move.x != 0 || move.y != 0 || move.z != 0) {
			move = Normalize(move);
			move = Multiply(moveSpeed, move);
		}
	}
	cameraPos->x += move.x;
	cameraPos->y += move.y;
	cameraPos->z += move.z;
}
// マウスでカメラを制御する時アイコンを表す
void MouseCameraDrawIcon(float windowWidth, float windowHeight, bool showHelpText) {
	windowWidth;
	if (showHelpText) {
		// Text
		float textLine = 17;
		Vector2 textPos{ 0 + 5, windowHeight - textLine * 5 - 5 };
		Novice::ScreenPrintf(int(textPos.x), int(textPos.y), "----- Mouse Camera Usage -----");
		Novice::ScreenPrintf(int(textPos.x), int(textPos.y + textLine * 1), "(The control logic is the same as Unity)");
		Novice::ScreenPrintf(int(textPos.x), int(textPos.y + textLine * 2), "Right mouse: camera rotation");
		Novice::ScreenPrintf(int(textPos.x), int(textPos.y + textLine * 3), "Middle mouse: camera movement and zoom in/out");
		Novice::ScreenPrintf(int(textPos.x), int(textPos.y + textLine * 4), "Right mouse + WASD: camera move");
	}
	// Icon
	int mouseX, mouseY;
	Novice::GetMousePosition(&mouseX, &mouseY);
	Vector2 currentMousePos{ float(mouseX), float(mouseY) };
	Vector2 eyeSize{ 9, 5 }, boxSize{ 3, 3 }, boxPos{ 9, 7 }; // マウスの右キーのアイコン
	Vector2 headSize{ 6, 7 }, fingerSize{ 3, 9 };           // マウスの中キーのアイコン
	if (Novice::IsPressMouse(1)) {
		// Eye
		Novice::DrawEllipse((int)currentMousePos.x, (int)currentMousePos.y, int(eyeSize.x), int(eyeSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawEllipse((int)currentMousePos.x, (int)currentMousePos.y, int(eyeSize.x + 1), int(eyeSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawEllipse((int)currentMousePos.x, (int)currentMousePos.y, int(eyeSize.y - 1), int(eyeSize.y - 1), 0, BLACK, kFillModeWireFrame);
		// MoveBox
		Novice::DrawBox(int(currentMousePos.x + boxPos.x), int(currentMousePos.y + boxPos.y), int(boxSize.x), int(boxSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x + boxPos.x), int(currentMousePos.y + boxPos.y), int(boxSize.x + 1), int(boxSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawBox(int(currentMousePos.x + boxPos.x), int(currentMousePos.y + boxPos.y + boxSize.y + 1), int(boxSize.x), int(boxSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x + boxPos.x), int(currentMousePos.y + boxPos.y + boxSize.y + 1), int(boxSize.x + 1), int(boxSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawBox(int(currentMousePos.x + boxPos.x - boxSize.x - 1), int(currentMousePos.y + boxPos.y + boxSize.y + 1), int(boxSize.x), int(boxSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x + boxPos.x - boxSize.x - 1), int(currentMousePos.y + boxPos.y + boxSize.y + 1), int(boxSize.x + 1), int(boxSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawBox(int(currentMousePos.x + boxPos.x + boxSize.x + 1), int(currentMousePos.y + boxPos.y + boxSize.y + 1), int(boxSize.x), int(boxSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x + boxPos.x + boxSize.x + 1), int(currentMousePos.y + boxPos.y + boxSize.y + 1), int(boxSize.x + 1), int(boxSize.y + 1), 0, BLACK, kFillModeWireFrame);
	}
	else if (Novice::IsPressMouse(2)) {
		// Finger
		Novice::DrawBox(int(currentMousePos.x - 5), int(currentMousePos.y - 13), int(fingerSize.x), int(fingerSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x - 5), int(currentMousePos.y - 13), int(fingerSize.x + 1), int(fingerSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawBox(int(currentMousePos.x - 8), int(currentMousePos.y - 7), int(fingerSize.x), int(fingerSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x - 8), int(currentMousePos.y - 7), int(fingerSize.x + 1), int(fingerSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawBox(int(currentMousePos.x - 1), int(currentMousePos.y - 14), int(fingerSize.x), int(fingerSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x - 1), int(currentMousePos.y - 14), int(fingerSize.x + 1), int(fingerSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawBox(int(currentMousePos.x + 6), int(currentMousePos.y - 10), int(fingerSize.x), int(fingerSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x + 6), int(currentMousePos.y - 10), int(fingerSize.x + 1), int(fingerSize.y + 1), 0, BLACK, kFillModeWireFrame);
		Novice::DrawBox(int(currentMousePos.x + 3), int(currentMousePos.y - 12), int(fingerSize.x), int(fingerSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawBox(int(currentMousePos.x + 3), int(currentMousePos.y - 12), int(fingerSize.x + 1), int(fingerSize.y + 1), 0, BLACK, kFillModeWireFrame);
		// Head
		Novice::DrawEllipse((int)currentMousePos.x + 2, (int)currentMousePos.y + 1, int(headSize.x), int(headSize.y), 0, WHITE, kFillModeSolid);
		Novice::DrawEllipse((int)currentMousePos.x + 2, (int)currentMousePos.y + 1, int(headSize.x + 1), int(headSize.y + 1), 0, BLACK, kFillModeWireFrame);
	}
}
#pragma endregion
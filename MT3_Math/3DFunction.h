#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include "Novice.h"


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
/// 加算
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
/// 減算
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
/// 乗算
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

/// <summary>
/// 定义 球的结构体
/// </summary>
typedef struct {
	Vector3 center;
	float radius;
}Sphere;

/// <summary>
/// 画球
/// </summary>
/// <param name="sphere">球的参数结构体</param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color">颜色</param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Matrix4x4 worldViewProjectionMatrix = viewProjectionMatrix;
	Matrix4x4 viewPortMatrix = viewportMatrix;

	const uint32_t kSubdivision = 20;	//分割数
	const float kLonEvery = 2.0f * float(M_PI) / float(kSubdivision);		//経度一つ分の角度
	const float kLatEvery = float(M_PI) / float(kSubdivision);		//緯度一つ分の角度

	Vector3 a[kSubdivision + 1];
	Vector3 b[kSubdivision + 1];
	Vector3 c[kSubdivision + 1];
	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * float(latIndex);		//現在の緯度
		//経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = kLonEvery * float(lonIndex);		//現在の経度
			//球面上の座標を求める
			a[latIndex].x = sphere.center.x + sphere.radius * cos(lat) * cos(lon);
			a[latIndex].y = sphere.center.y + sphere.radius * sin(lat);
			a[latIndex].z = sphere.center.z + sphere.radius * cos(lat) * sin(lon);

			b[latIndex].x = sphere.center.x + sphere.radius * cos(lat + kLatEvery) * cos(lon);
			b[latIndex].y = sphere.center.y + sphere.radius * sin(lat + kLatEvery);
			b[latIndex].z = sphere.center.z + sphere.radius * cos(lat + kLatEvery) * sin(lon);

			c[latIndex].x = sphere.center.x + sphere.radius * cos(lat) * cos(lon + kLonEvery);
			c[latIndex].y = sphere.center.y + sphere.radius * sin(lat);
			c[latIndex].z = sphere.center.z + sphere.radius * cos(lat) * sin(lon + kLonEvery);
			//画面上の座標を求める
			Vector3 ndcA = Transform(a[latIndex], worldViewProjectionMatrix);
			Vector3 ndcB = Transform(b[latIndex], worldViewProjectionMatrix);
			Vector3 ndcC = Transform(c[latIndex], worldViewProjectionMatrix);
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
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
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
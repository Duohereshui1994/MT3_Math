#define _USE_MATH_DEFINES
#include <Novice.h>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

void DrawGrid(const Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const uint32_t kSubdivision = 10;//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//一つ分の長さ
	Vector3 posHorizontalStart[kSubdivision + 1];
	Vector3 posHorizontalEnd[kSubdivision + 1];
	Vector3 posHorizontalStartScreen[kSubdivision + 1];
	Vector3 posHorizontalEndScreen[kSubdivision + 1];

	Matrix4x4 worldViewProjectionMatrix = viewProjectionMatrix;
	Matrix4x4 viewPortMatrix = viewportMatrix;
	//从深处到面前按顺序画线
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		posHorizontalStart[xIndex].y = kGridHalfWidth - xIndex * kGridEvery;
		posHorizontalStart[xIndex].x = -kGridHalfWidth;
		posHorizontalEnd[xIndex].y = kGridHalfWidth - xIndex * kGridEvery;
		posHorizontalEnd[xIndex].x = kGridHalfWidth;


		Vector3 ndcVertexHorizontalStart = Transform(posHorizontalStart[xIndex], worldViewProjectionMatrix);
		posHorizontalStartScreen[xIndex] = Transform(ndcVertexHorizontalStart, viewPortMatrix);
		Vector3 ndcVertexHorizontalEnd = Transform(posHorizontalEnd[xIndex], worldViewProjectionMatrix);
		posHorizontalEndScreen[xIndex] = Transform(ndcVertexHorizontalEnd, viewPortMatrix);
		Novice::DrawLine((int)posHorizontalStartScreen[xIndex].x, (int)posHorizontalStartScreen[xIndex].y, (int)posHorizontalEndScreen[xIndex].x, (int)posHorizontalEndScreen[xIndex].y, RED);

	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 rotate{ };
	Vector3 translate{};
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };
	Vector3 cameraPTranslate{ 0.0f,1.9f ,-6.49f };



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraPTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(projectionMatrix, viewPortMatrix);
#ifdef _DEBUG
		MatrixScreenPrintf(-0, kWindowHeight-600, worldViewProjectionMatrix, "worldViewProjectionMatrix");
		MatrixScreenPrintf(-0, kWindowHeight*6-600, viewPortMatrix, "viewPortMatrix");

#endif 


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

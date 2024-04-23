#define _USE_MATH_DEFINES
#include <Novice.h>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

void DrawGrid(const Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const uint32_t kSubdivision = 10;//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//一つ分の長さ

	Matrix4x4 worldViewProjectionMatrix = viewProjectionMatrix;
	Matrix4x4 viewPortMatrix = viewportMatrix;

	Vector3 startPos[kSubdivision + 1];
	Vector3 endPos[kSubdivision + 1];
	//Vector3 ndcStartPos[kSubdivision + 1];
	Vector3 startPosScreen[kSubdivision+1];
	Vector3 endPosScreen[kSubdivision+1];
	//从深处到面前按顺序画线
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//使用上面的信息，求出在world坐标里的起点和终点坐标
		startPos[xIndex].x = kGridHalfWidth;
		endPos[xIndex].x = -kGridHalfWidth;

		startPos[xIndex].y = 0.0f;
		endPos[xIndex].y = 0.0f;

		startPos[xIndex].z = kGridHalfWidth - xIndex * kGridEvery;
		endPos[xIndex].z = kGridHalfWidth - xIndex * kGridEvery;
		//Novice::ScreenPrintf(0, xIndex * 15, "pos[%d] = %f", xIndex, startPos[xIndex].z);
		//变换成screen坐标
		Vector3 ndcStartPos = Transform(startPos[xIndex], worldViewProjectionMatrix);
		Vector3 ndcEndPos = Transform(endPos[xIndex], worldViewProjectionMatrix);

		startPosScreen[xIndex] = Transform(ndcStartPos, viewPortMatrix);
		endPosScreen[xIndex] = Transform(ndcEndPos, viewPortMatrix);
		////使用变换后的坐标画线
		//
		Novice::DrawLine((int)startPosScreen[xIndex].x, (int)startPosScreen[xIndex].z, (int)endPosScreen[xIndex].x, (int)endPosScreen[xIndex].z,RED);

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

	Vector3 rotate{};
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
		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);
#ifdef _DEBUG
		MatrixScreenPrintf(0, kWindowHeight - 100, worldViewProjectionMatrix, "worldViewProjectionMatrix");

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

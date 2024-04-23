#define _USE_MATH_DEFINES
#include <Novice.h>
#include <cmath>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	float check = 0;
	Vector3 rotate{ };
	Vector3 translate{};
	Vector3 cameraPosition{ 0.0f,0.0f ,-10.0f };

	Vector3 kLocalVertices[3]{};
	kLocalVertices[0] = { 0.0f,1.0f,0.0f };
	kLocalVertices[1] = { 1.0f,-1.0f,0.0f };
	kLocalVertices[2] = { -1.f,-1.0f,0.0f };



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

		if (rotate.y < 2.0f * float(M_PI)) {
			rotate.y += 1.0f / 60 * float(M_PI);
		}
		else {
			rotate.y = 0;
		}

		if (keys[DIK_RIGHT]) {
			translate.x += 0.1f;
		}
		if (keys[DIK_LEFT]) {
			translate.x -= 0.1f;
		}
		if (keys[DIK_UP]) {
			cameraPosition.z += 0.1f;
		}
		if (keys[DIK_DOWN]) {
			cameraPosition.z -= 0.1f;
		}

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewPortMatrix);
		}

		Vector3 v1 = Subtract(screenVertices[1], screenVertices[0]);
		Vector3 v2 = Subtract(screenVertices[2], screenVertices[1]);
		Vector3 cross = Cross(v1, v2);

		check = Dot(cameraPosition, cross);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		VectorScreenPrintf(0, 0, cross, "Cross");
		if (check <= 0) {
			Novice::DrawTriangle((int)(screenVertices[0].x), (int)(screenVertices[0].y), (int)(screenVertices[1].x), (int)(screenVertices[1].y), (int)(screenVertices[2].x), (int)(screenVertices[2].y), RED, kFillModeSolid);
		}
#ifdef _DEBUG
		VectorScreenPrintf(0, 15, v1, "v1");
		VectorScreenPrintf(0, 30, v2, "v2");
		Novice::ScreenPrintf(0, 45, "check = %f", check);
		VectorScreenPrintf(0, 60, cameraPosition, "cameraPosition");

#endif // _DEBUG


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

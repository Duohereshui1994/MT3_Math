#include <Novice.h>
#include <cmath>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	int kWindowWidth = 1280;
	int kWindowHeight = 720;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };

	Vector3 rotate{0.0f,0.0f,0.0f};
	Vector3 translate{ 0.0f,0.0f,0.0f };
	Vector3 cameraPosition{ 640.0f,360.0f ,0.0f };

	Vector3 kLocalVertices[3]{};
	kLocalVertices[0] = { 100.0f,100.0f,50.0f };
	kLocalVertices[1] = { 50.0f,200.0f,50.0f };
	kLocalVertices[2] = { 150.0f,200.0f,50.0f };

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
		Vector3 cross = Cross(v1, v2);
		if (keys[DIK_D]) {
			translate.x += 5;
		}
		if (keys[DIK_A]) {
			translate.x -= 5;
		}
		if (keys[DIK_UP]) {
			translate.y += 5;
		}
		if (keys[DIK_DOWN]) {
			translate.y -= 5;
		}

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		//Matrix4x4 worldViewProjectionMatrix = Multiply(Multiply(worldMatrix, viewMatrix), projectionMatrix);
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewPortMatrix);
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		//VectorScreenPrintf(0, 0, cross, "Cross");
		Novice::DrawTriangle((int)(screenVertices[0].x), (int)(screenVertices[0].y), (int)(screenVertices[1].x), (int)(screenVertices[1].y), (int)(screenVertices[2].x), (int)(screenVertices[2].y), RED, kFillModeSolid);
		Novice::ScreenPrintf(0, 15, "screenVertices[0].x = %d,screenVertices[0].y = %d", screenVertices[0].x, screenVertices[0].y);
		Novice::ScreenPrintf(0, 30, "screenVertices[1].x = %d,screenVertices[1].y = %d", screenVertices[1].x, screenVertices[1].y);
		Novice::ScreenPrintf(0, 45, "screenVertices[2].x = %d,screenVertices[2].y = %d", screenVertices[2].x, screenVertices[2].y);
		//MatrixScreenPrintf(0, kRowHeight, worldMatrix, "worldMatrix");
		//MatrixScreenPrintf(0, kRowHeight*6, cameraMatrix, "cameraMatrix");
		//MatrixScreenPrintf(0, kRowHeight*11, viewMatrix, "viewMatrix");
		//MatrixScreenPrintf(0, kRowHeight*16, projectionMatrix, "projectionMatrix");
		MatrixScreenPrintf(kColumnWidth*8, kRowHeight, worldViewProjectionMatrix, "worldViewProjectionMatrix");
		MatrixScreenPrintf(kColumnWidth*8, kRowHeight * 6, viewPortMatrix, "viewPortMatrix");
		
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

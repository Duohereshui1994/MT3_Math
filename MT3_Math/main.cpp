#define NOMINMAX
#include <Novice.h>
#include <imgui.h>
#include <algorithm>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

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
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };


	AABB aabb1{ {-0.5f,-0.5f,-0.5f},{0.0f,0.0f,0.0f} };
	Sphere sphere{ {0.5f,0.5f,0.5f},0.5f };



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

#ifdef _DEBUG

		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		//MouseCameraDrawIcon(1280, 720, true);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("AABB1 max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("AABB1 min", &aabb1.min.x, 0.01f);

		ImGui::DragFloat3("Sphere center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere radius", &sphere.radius, 0.01f);



		ImGui::End();

#endif 

		CorrectAABB2(aabb1);
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
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


		DrawSphere(sphere, worldViewProjectionMatrix, viewPortMatrix, WHITE);

		if (IsCollision(aabb1, sphere)) {
			DrawAABB(aabb1, worldViewProjectionMatrix, viewPortMatrix, RED);
		}
		else {
			DrawAABB(aabb1, worldViewProjectionMatrix, viewPortMatrix, WHITE);
		}




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

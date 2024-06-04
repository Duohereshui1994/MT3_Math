#define NOMINMAX
#include <Novice.h>
#include <imgui.h>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

/// <summary>
/// AABB结构体
/// </summary>
typedef struct {
	Vector3 min;
	Vector3 max;
}AABB;

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
/// 画AABB
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewPortMatrix"></param>
/// <param name="color"></param>
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

	AABB aabb2{ {0.2f,0.2f,0.2f},{1.0f,1.0f,1.0f} };



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
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		CorrectAABB(aabb1);
		CorrectAABB(aabb2);



#ifdef _DEBUG

		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		//MouseCameraDrawIcon(1280, 720, true);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("AABB1 max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("AABB1 min", &aabb1.min.x, 0.01f);

		ImGui::DragFloat3("AABB2 max", &aabb2.max.x, 0.01f);
		ImGui::DragFloat3("AABB2 min", &aabb2.min.x, 0.01f);


		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);
		if (IsCollision(aabb1, aabb2)) {
			DrawAABB(aabb1, worldViewProjectionMatrix, viewPortMatrix, RED);
		}
		else {
			DrawAABB(aabb1, worldViewProjectionMatrix, viewPortMatrix, WHITE);
		}
		DrawAABB(aabb2, worldViewProjectionMatrix, viewPortMatrix, WHITE);




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

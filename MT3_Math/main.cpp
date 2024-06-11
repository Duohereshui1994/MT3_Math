#define NOMINMAX
#include <Novice.h>
#include <imgui.h>
#include <algorithm>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

/// <summary>
/// 线性插值函数
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Add(Multiply((1.0f - t), v1), Multiply(t, v2));
}

/// <summary>
/// 画3次贝塞尔曲线
/// </summary>
/// <param name="controlPoint0"></param>
/// <param name="controlPoint1"></param>
/// <param name="controlPoint2"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
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


	Vector3 controlPoints[3] = {
		{ -0.8f,0.58f,1.0f },
		{ 1.76f,1.0f,-0.3f },
		{ 0.94f,-0.7f,2.3f }
	};

	Sphere pointSphere[3];


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

		pointSphere[0] = { controlPoints[0],0.01f };
		pointSphere[1] = { controlPoints[1],0.01f };
		pointSphere[2] = { controlPoints[2],0.01f };


#ifdef _DEBUG

		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		//MouseCameraDrawIcon(1280, 720, true);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("ControlPoint 1", &controlPoints[0].x, 0.01f);
		ImGui::DragFloat3("ControlPoint 2", &controlPoints[1].x, 0.01f);
		ImGui::DragFloat3("ControlPoint 3", &controlPoints[2].x, 0.01f);

		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);


		DrawBezier(controlPoints[0], controlPoints[1], controlPoints[2], worldViewProjectionMatrix, viewPortMatrix, BLUE);
		DrawSphere(pointSphere[0], worldViewProjectionMatrix, viewPortMatrix, RED);
		DrawSphere(pointSphere[1], worldViewProjectionMatrix, viewPortMatrix, RED);
		DrawSphere(pointSphere[2], worldViewProjectionMatrix, viewPortMatrix, RED);




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

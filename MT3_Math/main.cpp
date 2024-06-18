#define NOMINMAX
#include <Novice.h>
#include <imgui.h>
#include <algorithm>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

// 圆锥振子
typedef struct {
	Vector3 anchor;             // 固定点
	float length;               // 绳长
	float halfApexAngle;        // 圆锥顶角的一半
	float angle;                // 角度
	float angularVelocity;      // 角速度
}ConicalPendulum;

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

	bool isStart = false;

	float deltaTime = 1.0f / 60.0f;

	ConicalPendulum conicalPendulum{
		.anchor = { 0.0f, 1.0f, 0.0f },
		.length = 0.8f,
		.halfApexAngle = 0.7f,
		.angle = 0.0f,
		.angularVelocity = 0.0f,
	};

	Ball ball{
		.position = { 1.2f, 0.0f, 0.0f },
		.mass = 2.0f,
		.radius = 0.05f,
		.color = WHITE,
	};


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

		//视角矩阵渲染方法
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		if (isStart)
		{
			// 角速度計算
			conicalPendulum.angularVelocity = std::sqrt((9.8f * std::cos(conicalPendulum.halfApexAngle)) / (conicalPendulum.length * std::sin(conicalPendulum.halfApexAngle) * std::sin(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
		}

		// 圆周运动位置計算
		float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		ball.position.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
		ball.position.y = conicalPendulum.anchor.y - height;
		ball.position.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;


		Vector3 anchorScreenPos = Transform(Transform(conicalPendulum.anchor, worldViewProjectionMatrix), viewPortMatrix);
		Vector3 ballScreenPos = Transform(Transform(ball.position, worldViewProjectionMatrix), viewPortMatrix);

#ifdef _DEBUG

		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		//MouseCameraDrawIcon(1280, 720, true);

		ImGui::Begin("Window");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::Button("Start");
		if (ImGui::IsItemClicked()) {
			isStart = true;
		}

		ImGui::Button("Reset");
		if (ImGui::IsItemClicked()) {
			isStart = false;
			conicalPendulum.angle = 0.0f;
		}

		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);

		DrawSphere(Sphere{ ball.position, ball.radius }, worldViewProjectionMatrix, viewPortMatrix, ball.color);

		Novice::DrawLine((int)anchorScreenPos.x, (int)anchorScreenPos.y, (int)ballScreenPos.x, (int)ballScreenPos.y, WHITE);


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

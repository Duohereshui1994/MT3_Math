#define NOMINMAX
#include <Novice.h>
#include <imgui.h>
#include <algorithm>
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

	Vector3 rotate{};
	Vector3 translate{};
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	//0:肩 1：肘 2：手
	Vector3 translates[3] = { { 0.2f,1.0f,0.0f }, { 0.4f,0.0f,0.0f }, { 0.3f,0.0f,0.0f } };
	Vector3 rotates[3] = { { 0.0f,0.0f,-6.8f }, { 0.0f,0.0f,-1.4f }, { 0.0f,0.0f,0.0f } };
	Vector3 scales[3] = { { 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f } };


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

		//肩 肘 手 坐标计算

		Matrix4x4 shoulderMatrix = MakeAffineMatrix(scales[0], rotates[0], translates[0]); //肩的英文 ls
		Matrix4x4 elbowMatrix = MakeAffineMatrix(scales[1], rotates[1], translates[1]);//手肘的英文 le
		Matrix4x4 handMatrix = MakeAffineMatrix(scales[2], rotates[2], translates[2]);//手的英文 lh

		Matrix4x4 shoulderWorldMatrix = shoulderMatrix;//Ws
		Matrix4x4 elbowWorldMatrix = Multiply(elbowMatrix, shoulderMatrix);//肩-肘系统的坐标 We
		Matrix4x4 handWorldMatrix = Multiply(Multiply(handMatrix, elbowMatrix), shoulderMatrix);//肘-手系统的坐标 Wh

		Vector3 shoulderPos = { shoulderWorldMatrix.m[3][0], shoulderWorldMatrix.m[3][1], shoulderWorldMatrix.m[3][2] };
		Vector3 elbowPos = { elbowWorldMatrix.m[3][0], elbowWorldMatrix.m[3][1], elbowWorldMatrix.m[3][2] };
		Vector3 handPos = { handWorldMatrix.m[3][0], handWorldMatrix.m[3][1], handWorldMatrix.m[3][2] };

		Vector3 shoulderScreenPos = Transform(Transform(shoulderPos, worldViewProjectionMatrix), viewPortMatrix);
		Vector3 elbowScreenPos = Transform(Transform(elbowPos, worldViewProjectionMatrix), viewPortMatrix);
		Vector3 handScreenPos = Transform(Transform(handPos, worldViewProjectionMatrix), viewPortMatrix);



#ifdef _DEBUG

		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		//MouseCameraDrawIcon(1280, 720, true);

		ImGui::Begin("Window");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("ShoulderPos", &translates[0].x, 0.01f);
		ImGui::DragFloat3("ShoulderRotate", &rotates[0].x, 0.01f);

		ImGui::DragFloat3("ElbowPos", &translates[1].x, 0.01f);
		ImGui::DragFloat3("ElbowRotate", &rotates[1].x, 0.01f);

		ImGui::DragFloat3("HandPos", &translates[2].x, 0.01f);
		ImGui::DragFloat3("HandRotate", &rotates[2].x, 0.01f);

		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);

		Novice::DrawLine((int)shoulderScreenPos.x, (int)shoulderScreenPos.y, (int)elbowScreenPos.x, (int)elbowScreenPos.y, WHITE);
		Novice::DrawLine((int)elbowScreenPos.x, (int)elbowScreenPos.y, (int)handScreenPos.x, (int)handScreenPos.y, WHITE);

		DrawSphere(Sphere{ shoulderPos, 0.1f }, worldViewProjectionMatrix, viewPortMatrix, RED);
		DrawSphere(Sphere{ elbowPos, 0.1f }, worldViewProjectionMatrix, viewPortMatrix, GREEN);
		DrawSphere(Sphere{ handPos, 0.1f }, worldViewProjectionMatrix, viewPortMatrix, BLUE);




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

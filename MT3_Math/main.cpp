#define NOMINMAX
#include <Novice.h>
#include <imgui.h>
#include <algorithm>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

//弹簧
typedef struct
{
	Vector3 anchor;				//固定端的位置
	float naturalLength;		//自然长度
	float stiffness;			//刚性系数
	float dampingCoefficirent;	//衰减系数
}Spring;

//球
typedef struct
{
	Vector3 position;		//位置
	Vector3 velocity;		//速度
	Vector3 acceleration;	//加速度
	float mass;				//质量
	float radius;			//半径
	unsigned int color;		//颜色
}Ball;



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

	Spring spring{
		.anchor = { 0.0f, 0.0f, 0.0 },
		.naturalLength = 1.0f ,
		.stiffness = 100.0f,
		.dampingCoefficirent = 2.0f,
	};


	Ball ball{
		.position = { 1.2f, 0.0f, 0.0f },
		.mass = 2.0f,
		.radius = 0.05f,
		.color = BLUE,
	};

	float deltaTime = 1.0f / 60.0f;

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

		Vector3 diff = ball.position - spring.anchor;
		float length = Length(diff);
		if (length != 0.0f) {
			Vector3 direction = Normalize(diff);
			Vector3 restPostion = spring.anchor + direction * spring.naturalLength;
			Vector3 displacement = length * (ball.position - restPostion);
			Vector3 restoringForce = -spring.stiffness * displacement;
			Vector3 dampingForce = -spring.dampingCoefficirent * ball.velocity;
			Vector3 force = restoringForce + dampingForce;
			ball.acceleration = force / ball.mass;
		}
		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;


		Vector3 anchorScreenPos = Transform(Transform(spring.anchor, worldViewProjectionMatrix), viewPortMatrix);
		Vector3 ballScreenPos = Transform(Transform(ball.position, worldViewProjectionMatrix), viewPortMatrix);

#ifdef _DEBUG

		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		//MouseCameraDrawIcon(1280, 720, true);

		ImGui::Begin("Window");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);



		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);

		DrawSphere(Sphere{ .center = ball.position,.radius = ball.radius }, worldViewProjectionMatrix, viewPortMatrix, ball.color);

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

#define NOMINMAX
#include <Novice.h>
#include <imgui.h>
#include <algorithm>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

//弹簧
//バネの構造体
typedef struct
{
	Vector3 anchor;				//固定端的位置	アンカー。固定された端の位置
	float naturalLength;		//自然长度	自然長さ
	float stiffness;			//刚性系数	剛性、バネ定数ｋ
	float dampingCoefficirent;	//衰减系数	減衰係数
}Spring;

//球
//ボールの構造体
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

	//回転
	Vector3 rotate{};
	//移動
	Vector3 translate{};
	//カメラの回転
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };
	//カメラの移動
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	//構造体初期化
	Spring spring{
		.anchor = { 0.0f, 0.0f, 0.0 },
		.naturalLength = 1.0f ,
		.stiffness = 100.0f,
		.dampingCoefficirent = 2.0f,
	};

	//構造体初期化
	Ball ball{
		.position = { 1.2f, 0.0f, 0.0f },
		.mass = 2.0f,
		.radius = 0.05f,
		.color = BLUE,
	};

	//時間間隔
	float deltaTime = 1.0f / 60.0f;

	//フレーム開始フラグ
	bool isStart = false;

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
		
		//フラグ
		if (isStart) {
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
		}

		//视角矩阵渲染方法	レンダリング
		//各種行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		// 透視投影行列を作成
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		//ビュー座標変換行列を作成
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		//ViewportMatrixビューポート変換行列を作成
		Matrix4x4 viewPortMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		//スクリーン座標変換
		Vector3 anchorScreenPos = Transform(Transform(spring.anchor, worldViewProjectionMatrix), viewPortMatrix);
		Vector3 ballScreenPos = Transform(Transform(ball.position, worldViewProjectionMatrix), viewPortMatrix);

#ifdef _DEBUG
		//デバッグ用カメラ
		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		//ImGuiの制御
		ImGui::Begin("Window");
		ImGui::Button("Start");
		if (ImGui::IsItemClicked()) {
			isStart = true;
		}
		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// Gridを描画
		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);
		// Ballを描画
		DrawSphere(Sphere{ .center = ball.position,.radius = ball.radius }, worldViewProjectionMatrix, viewPortMatrix, ball.color);
		// バネのアンカーとボールの位置を結ぶ線を描画
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

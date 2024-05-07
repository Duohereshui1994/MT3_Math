#include <Novice.h>
#include <imgui.h>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

typedef struct {
	Vector3 center;
	float radius;
}Sphere;

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Matrix4x4 worldViewProjectionMatrix = viewProjectionMatrix;
	Matrix4x4 viewPortMatrix = viewportMatrix;

	const uint32_t kSubdivision = 20;	//分割数
	const float kLonEvery = 2.0f * float(M_PI) / float(kSubdivision);		//経度一つ分の角度
	const float kLatEvery = float(M_PI) / float(kSubdivision);		//緯度一つ分の角度

	Vector3 a[kSubdivision + 1];
	Vector3 b[kSubdivision + 1];
	Vector3 c[kSubdivision + 1];
	//緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * float(latIndex);		//現在の緯度
		//経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = kLonEvery * float(lonIndex);		//現在の経度
			//球面上の座標を求める
			a[latIndex].x = sphere.center.x + sphere.radius * cos(lat) * cos(lon);
			a[latIndex].y = sphere.center.y + sphere.radius * sin(lat);
			a[latIndex].z = sphere.center.z + sphere.radius * cos(lat) * sin(lon);

			b[latIndex].x = sphere.center.x + sphere.radius * cos(lat + kLatEvery) * cos(lon);
			b[latIndex].y = sphere.center.y + sphere.radius * sin(lat + kLatEvery);
			b[latIndex].z = sphere.center.z + sphere.radius * cos(lat + kLatEvery) * sin(lon);

			c[latIndex].x = sphere.center.x + sphere.radius * cos(lat) * cos(lon + kLonEvery);
			c[latIndex].y = sphere.center.y + sphere.radius * sin(lat);
			c[latIndex].z = sphere.center.z + sphere.radius * cos(lat) * sin(lon + kLonEvery);
			//画面上の座標を求める
			Vector3 ndcA = Transform(a[latIndex], worldViewProjectionMatrix);
			Vector3 ndcB = Transform(b[latIndex], worldViewProjectionMatrix);
			Vector3 ndcC = Transform(c[latIndex], worldViewProjectionMatrix);
			Vector3 screenA = Transform(ndcA, viewPortMatrix);
			Vector3 screenB = Transform(ndcB, viewPortMatrix);
			Vector3 screenC = Transform(ndcC, viewPortMatrix);

			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenB.x, (int)screenB.y, color);
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenC.x, (int)screenC.y, color);
		}
	}
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;//Gridの半分の幅
	const uint32_t kSubdivision = 10;//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);//一つ分の長さ

	Matrix4x4 worldViewProjectionMatrix = viewProjectionMatrix;
	Matrix4x4 viewPortMatrix = viewportMatrix;

	Vector3 startPosHorizontal[kSubdivision + 1];
	Vector3 endPosHorizontal[kSubdivision + 1];
	Vector3 startPosScreenHorizontal[kSubdivision + 1];
	Vector3 endPosScreenHorizontal[kSubdivision + 1];

	Vector3 startPosVertical[kSubdivision + 1];
	Vector3 endPosVertical[kSubdivision + 1];
	Vector3 startPosScreenVertical[kSubdivision + 1];
	Vector3 endPosScreenVertical[kSubdivision + 1];
	//从深处到面前按顺序画线
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		//使用上面的信息，求出在world坐标里的起点和终点坐标
		startPosHorizontal[xIndex].x = kGridHalfWidth;
		endPosHorizontal[xIndex].x = -kGridHalfWidth;

		startPosHorizontal[xIndex].y = 0.0f;
		endPosHorizontal[xIndex].y = 0.0f;

		startPosHorizontal[xIndex].z = kGridHalfWidth - xIndex * kGridEvery;
		endPosHorizontal[xIndex].z = kGridHalfWidth - xIndex * kGridEvery;
		//变换成screen坐标
		Vector3 ndcStartPosHorizontal = Transform(startPosHorizontal[xIndex], worldViewProjectionMatrix);
		Vector3 ndcEndPosHorizontal = Transform(endPosHorizontal[xIndex], worldViewProjectionMatrix);

		startPosScreenHorizontal[xIndex] = Transform(ndcStartPosHorizontal, viewPortMatrix);
		endPosScreenHorizontal[xIndex] = Transform(ndcEndPosHorizontal, viewPortMatrix);
		////使用变换后的坐标画线

		if (xIndex == 5) {
			Novice::DrawLine((int)startPosScreenHorizontal[xIndex].x, (int)startPosScreenHorizontal[xIndex].y, (int)endPosScreenHorizontal[xIndex].x, (int)endPosScreenHorizontal[xIndex].y, BLACK);
		}
		else {
			Novice::DrawLine((int)startPosScreenHorizontal[xIndex].x, (int)startPosScreenHorizontal[xIndex].y, (int)endPosScreenHorizontal[xIndex].x, (int)endPosScreenHorizontal[xIndex].y, 0xAAAAAAFF);
		}

	}
	//从左到右
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		startPosVertical[zIndex].x = kGridHalfWidth - zIndex * kGridEvery;
		endPosVertical[zIndex].x = kGridHalfWidth - zIndex * kGridEvery;

		startPosVertical[zIndex].y = 0.0f;
		endPosVertical[zIndex].y = 0.0f;

		startPosVertical[zIndex].z = kGridHalfWidth;
		endPosVertical[zIndex].z = -kGridHalfWidth;

		Vector3 ndcStartPosVertical = Transform(startPosVertical[zIndex], worldViewProjectionMatrix);
		Vector3 ndcEndPosVertical = Transform(endPosVertical[zIndex], worldViewProjectionMatrix);

		startPosScreenVertical[zIndex] = Transform(ndcStartPosVertical, viewPortMatrix);
		endPosScreenVertical[zIndex] = Transform(ndcEndPosVertical, viewPortMatrix);

		if (zIndex == 5) {
			Novice::DrawLine((int)startPosScreenVertical[zIndex].x, (int)startPosScreenVertical[zIndex].y, (int)endPosScreenVertical[zIndex].x, (int)endPosScreenVertical[zIndex].y, BLACK);
		}
		else {
			Novice::DrawLine((int)startPosScreenVertical[zIndex].x, (int)startPosScreenVertical[zIndex].y, (int)endPosScreenVertical[zIndex].x, (int)endPosScreenVertical[zIndex].y, 0xAAAAAAFF);
		}

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
	Vector3 cameraTranslate{ 0.0f,1.9f ,-6.49f };

	Sphere sphere;
	sphere.center = Vector3{ 0.0f,0.0f,0.0f };
	sphere.radius = 0.5f;



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

#ifdef _DEBUG
		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);	
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);
		DrawSphere(sphere, worldViewProjectionMatrix, viewPortMatrix, BLACK);
#ifdef _DEBUG


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

#include <Novice.h>
#include <imgui.h>
#include "3DFunction.h"

const char kWindowTitle[] = "GC2A_04_ゴ_ウ";

/// <summary>
/// 三角形结构体
/// </summary>
typedef struct {
	Vector3 vertices[3];
}Triangle;

/// <summary>
/// 冲突判定 三角形和线
/// </summary>
/// <param name="triangle"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment) {

	//计算三角形所在平面
	Vector3 ab = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 ac = Subtract(triangle.vertices[2], triangle.vertices[0]);
	Vector3 normal = Cross(ab, ac);
	float distance = Dot(normal, triangle.vertices[0]);


	float dot = Dot(normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}

	//计算t
	float t = (distance - Dot(normal, segment.origin)) / dot;
	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	//计算交点在不在三角形内
	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));
	Vector3 crossAB = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
	Vector3 crossBC = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
	Vector3 crossCA = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));
	if (Dot(crossAB, normal) >= 0 && Dot(crossBC, normal) >= 0 && Dot(crossCA, normal) >= 0) {
		return true;
	}
	return false;
}


/// <summary>
/// 画三角形
/// </summary>
/// <param name="triangle"></param>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewPortMatrix"></param>
/// <param name="color"></param>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {
	Vector3 screenVertices[3];
	for (uint32_t i = 0; i < 3; ++i) {
		Vector3 ndcVertex = Transform(triangle.vertices[i], viewProjectionMatrix);
		screenVertices[i] = Transform(ndcVertex, viewPortMatrix);
	}
	Novice::DrawTriangle((int)(screenVertices[0].x), (int)(screenVertices[0].y), (int)(screenVertices[1].x), (int)(screenVertices[1].y), (int)(screenVertices[2].x), (int)(screenVertices[2].y), color, kFillModeWireFrame);
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

	Segment segment{ {0.0f, -0.0f,-1.0f},{0.0f,0.0f,1.0f} };
	Triangle triangle;
	triangle.vertices[0] = { -1.0f, 0.0f, 0.0f };
	triangle.vertices[1] = { 1.0f, 0.0f, 0.0f };
	triangle.vertices[2] = { 0.0f, 1.0f, 0.0f };





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


		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewPortMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), worldViewProjectionMatrix), viewPortMatrix);


#ifdef _DEBUG

		MouseCamera(&cameraTranslate, &cameraRotate, keys);

		MouseCameraDrawIcon(1280, 720, true);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);


		ImGui::DragFloat3("SegmentTranslate", &segment.origin.x, 0.01f);


		ImGui::End();

#endif 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewPortMatrix);

		if (IsCollision(triangle, segment)) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), RED);
		}
		else {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);
		}

		DrawTriangle(triangle, worldViewProjectionMatrix, viewPortMatrix, WHITE);




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

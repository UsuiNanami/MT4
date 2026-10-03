#include "Novice.h"
#include "imgui.h"
#include "MyMath.h"

const char kWindowTitle[] = "LE2B_ウスイ_ナナミ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	// 追従補間用の変数
	const float kDeltaTime = 1.0f / 60.0f;

	// 円赤 カーソル	
	const int kRadiusA = 12;
	Vector3 target = {0.0f, 0.0f, 0.0f};

	// 円緑 追従
	const int kRadiusB = 20;
	Vector3 pos = {640.0f, 360.0f, 0.0f};

	// 追従速度
	float speed = 6.0f;


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

		int mouseX = 0;
		int mouseY = 0;
		Novice::GetMousePosition(&mouseX, &mouseY);
		target = {static_cast<float>(mouseX), static_cast<float>(mouseY), 0.0f};
		
		pos += (speed * kDeltaTime) * (target - pos);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		ImGui::Begin("Interpolation Controller");
		ImGui::Text("Target: Mouse Position (Red Circle)");
		ImGui::SliderFloat("Speed", &speed, 0.0f, 60.0f);
		ImGui::Text("Mouse Pos: (%.1f, %.1f)", target.x, target.y);
		ImGui::Text("Circle Pos: (%.1f, %.1f)", pos.x, pos.y);
		ImGui::Text("Distance to target: %.1f px", MyMath::Length(target - pos));
		ImGui::End();

		Novice::DrawLine(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(target.x), static_cast<int>(target.y), WHITE);
		Novice::DrawEllipse(static_cast<int>(pos.x), static_cast<int>(pos.y), kRadiusB, kRadiusB, 0.0f, GREEN, kFillModeSolid);
		Novice::DrawEllipse(static_cast<int>(target.x), static_cast<int>(target.y), kRadiusA, kRadiusA, 0.0f, RED, kFillModeSolid);

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

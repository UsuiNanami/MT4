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

	// 球面座標の初期化 (r=6, theta=0, phi=-pi/2)
	const float halfPi = std::numbers::pi_v<float> / 2.0f;
	Spherical spherical{6.0f, 0.0f, -halfPi};

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

		// 球面座標の更新
		Vector3 pos = MyMath::ToCartesian(spherical);
		Matrix4x4 cameraMatrix = MyMath::MakeCameraMatrix(spherical);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// ImGuiの描画開始
		ImGui::Begin("Spherical Coordinates");

		ImGui::Text("Target: (0, 0, 0) / +Y up / Camera +Z forward");
		ImGui::Separator();

		// 球面座標の編集
		ImGui::InputFloat("Radius", &spherical.radius, 0.1f);
		ImGui::InputFloat("Theta: elevation (rad)", &spherical.theta, 0.01f);
		ImGui::InputFloat("Phi (rad)", &spherical.phi, 0.01f);

		ImGui::Separator();

		ImGui::Text("Spherical: r = %.3f, theta = %.3f rad, phi = %.3f rad", spherical.radius, spherical.theta, spherical.phi);
		ImGui::Text("Cartesian: x = %.3f, y = %.3f, z = %.3f", pos.x, pos.y, pos.z);

		ImGui::Separator();

		// 4x4 カメラ行列の数値表示
		ImGui::Text("Camera matrix");
		for (int row = 0; row < 4; ++row) {
			ImGui::Text("  %7.3f %7.3f %7.3f %7.3f", cameraMatrix.m[row][0], cameraMatrix.m[row][1], cameraMatrix.m[row][2], cameraMatrix.m[row][3]);
		}

		ImGui::End();


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

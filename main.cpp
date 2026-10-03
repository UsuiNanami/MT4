#include "Novice.h"
#include "imgui.h"
#include "MyMath.h"

const char kWindowTitle[] = "LE2B_ウスイ_ナナミ";

static const int kRowHeight = 20;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) { Novice::ScreenPrintf(x, y, "%.2f  %.2f  %.2f  %s", vector.x, vector.y, vector.z, label); }

void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* label) { Novice::ScreenPrintf(x, y, "%.2f  %.2f  %.2f  %.2f  %s", q.x, q.y, q.z, q.w, label); }

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		Novice::ScreenPrintf(x, y + (row + 1) * kRowHeight, "%.3f  %.3f  %.3f  %.3f", matrix.m[row][0], matrix.m[row][1], matrix.m[row][2], matrix.m[row][3]);
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 from0 = MyMath::Normalize(Vector3{1.0f, 0.7f, 0.5f});
	Vector3 to0 = -from0;
	Vector3 from1 = MyMath::Normalize(Vector3{-0.6f, 0.9f, 0.2f});
	Vector3 to1 = MyMath::Normalize(Vector3{0.4f, 0.7f, -0.5f});

	// DirectionToDirectionでクォータニオンを作り、それを回転行列（MakeRotateMatrix）に変換する
	Matrix4x4 rotateMatrix0 = MakeRotateMatrix(DirectionToDirection(MyMath::Normalize(Vector3{1.0f, 0.0f, 0.0f}), MyMath::Normalize(Vector3{-1.0f, 0.0f, 0.0f})));
	Matrix4x4 rotateMatrix1 = MakeRotateMatrix(DirectionToDirection(from0, to0));
	Matrix4x4 rotateMatrix2 = MakeRotateMatrix(DirectionToDirection(from1, to1));

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


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, kRowHeight * 0, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");

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

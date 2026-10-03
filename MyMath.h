#pragma once
#include <cmath>
#include <numbers>
#include <algorithm>

// 座標系の定義
struct Vector3 { // 3次元ベクトル
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};
struct Matrix4x4 { // 4x4行列
	float m[4][4] = {0.0f};
};

struct Spherical {
	float radius = 1.0f; // 距離 r
	float theta = 0.0f;  // 仰角 θ [rad]
	float phi = 0.0f;    // 方位角 φ [rad]
};

struct Cylindrical {
	float radius = 1.0f; // 水平の半径 r
	float theta = 0.0f;  // 角度 θ [rad]
	float height = 0.0f; // 高さ z
};

struct Polar2D {
	float radius = 1.0f; // 距離 r
	float theta = 0.0f;  // 角度 θ [rad]
};

class MyMath {
public:
	// ベクトル算術ヘルパー
	static Vector3 Subtract(const Vector3& a, const Vector3& b);
	static float Length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Cross(const Vector3& a, const Vector3& b);

	// 座標変換関数
	static Vector3 ToCartesian(const Spherical& s);
	static Vector3 ToCartesian(const Cylindrical& c);
	static Vector3 ToCartesian(const Polar2D& p);

	// 重心座標補間
	static Vector3 GetBarycentricPosition(const Vector3& a, const Vector3& b, const Vector3& c, float u, float v, float w);

	// カメラ行列生成関数
	static Matrix4x4 MakeCameraMatrix(const Spherical& spherical);
};
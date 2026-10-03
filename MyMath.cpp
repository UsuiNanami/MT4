#include "MyMath.h"

// --- ベクトル算術ヘルパー ---

// ベクトルの引き算 (a - b)
Vector3 MyMath::Subtract(const Vector3& a, const Vector3& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

// ベクトルの長さ（ノルム）
float MyMath::Length(const Vector3& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

// ベクトルの正規化（単位ベクトル化）
Vector3 MyMath::Normalize(const Vector3& v) {
	float len = Length(v);
	if (len < 1e-6f)
		return {0.0f, 0.0f, 0.0f}; 
	return {v.x / len, v.y / len, v.z / len};
}

// ベクトルの外積 (a × b)
Vector3 MyMath::Cross(const Vector3& a, const Vector3& b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

// --- 座標変換関数 ---

// 球面座標 -> 直交座標 (x, y, z)
Vector3 MyMath::ToCartesian(const Spherical& s) {
	float cosTheta = std::cos(s.theta);
	return {s.radius * cosTheta * std::cos(s.phi), s.radius * std::sin(s.theta), s.radius * cosTheta * std::sin(s.phi)};
}

// 円筒座標 -> 直交座標 (x, y, z)
Vector3 MyMath::ToCartesian(const Cylindrical& c) { return {c.radius * std::cos(c.theta), c.height, c.radius * std::sin(c.theta)}; }

// 2D極座標 -> 直交座標 (x, y, 0)
Vector3 MyMath::ToCartesian(const Polar2D& p) { return {p.radius * std::cos(p.theta), p.radius * std::sin(p.theta), 0.0f}; }

// 重心座標補間 (P = uA + vB + wC)
Vector3 MyMath::GetBarycentricPosition(const Vector3& a, const Vector3& b, const Vector3& c, float u, float v, float w) {
	return {u * a.x + v * b.x + w * c.x, u * a.y + v * b.y + w * c.y, u * a.z + v * b.z + w * c.z};
}

// --- カメラ行列生成関数 ---
Matrix4x4 MyMath::MakeCameraMatrix(const Spherical& sphericalInput) {
	// 特異点回避のためのクランプ処理
	Spherical s = sphericalInput;
	const float limit = std::numbers::pi_v<float> / 2.0f - 0.001f; // ±π/2 直前で固定
	s.radius = std::max(s.radius, 0.001f);
	s.theta = std::clamp(s.theta, -limit, limit);

	// カメラ位置 Eye を求める
	Vector3 eye = ToCartesian(s);
	Vector3 target = {0.0f, 0.0f, 0.0f}; // 注視点（原点）

	// 注視点への前 F、世界の上との外積で右 R、さらに F と R の外積で上 U
	// 注視点への前軸 Forward (Target - Eye)
	Vector3 forward = Normalize(Subtract(target, eye));

	// 世界の上軸 WorldUp (0, 1, 0)
	Vector3 worldUp = {0.0f, 1.0f, 0.0f};

	// 右軸 Right (WorldUp × Forward)
	Vector3 right = Normalize(Cross(worldUp, forward));

	// 上軸 Up (Forward × Right)
	Vector3 up = Cross(forward, right);

	// 求めた右・上・前とカメラ位置を行列に格納する (4x4 ワールド行列)
	Matrix4x4 mat{};
	mat.m[0][0] = right.x;
	mat.m[0][1] = right.y;
	mat.m[0][2] = right.z;
	mat.m[0][3] = 0.0f;
	mat.m[1][0] = up.x;
	mat.m[1][1] = up.y;
	mat.m[1][2] = up.z;
	mat.m[1][3] = 0.0f;
	mat.m[2][0] = forward.x;
	mat.m[2][1] = forward.y;
	mat.m[2][2] = forward.z;
	mat.m[2][3] = 0.0f;
	mat.m[3][0] = eye.x;
	mat.m[3][1] = eye.y;
	mat.m[3][2] = eye.z;
	mat.m[3][3] = 1.0f;

	return mat;
}
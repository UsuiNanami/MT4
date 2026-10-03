#pragma once
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdint.h>

// ============================================
// 基本構造体・座標系定義
// ============================================
struct Vector3 { // 3次元ベクトル
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	// --- 複合代入演算子 ---
	Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& operator*=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3& operator/=(float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
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

struct Sphere {     // 球
	Vector3 center; // 中心
	float radius;   // 半径
};

struct Line {       // 線分
	Vector3 origin; // 始点
	Vector3 diff;   // 方向ベクトル
};

struct Ray {        // 光線
	Vector3 origin; // 始点
	Vector3 diff;   // 方向ベクトル
};

struct Segment {    // 線分
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};

struct Plane {      // 平面
	Vector3 normal; // 法線ベクトル
	float distance; // 原点から平面までの距離
};

struct Triangle {        // 三角形
	Vector3 vertices[3]; // 原点
};

struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};

// ============================================
// 描画関数
// ============================================
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);       // 球を描画する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);                                               // グリッドを描画する関数
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);          // 平面を描画する関数
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);    // 線分を描画する関数
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color); // 三角形を描画する関数
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);             // AABBを描画する関数

// ============================================
// 演算子オーバーロード
// ============================================
// Vector3 二項演算子
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(float s, const Vector3& v);
Vector3 operator*(const Vector3& v, float s);
Vector3 operator/(const Vector3& v, float s);

// Vector3 単項演算子
Vector3 operator-(const Vector3& v);
Vector3 operator+(const Vector3& v);

// Matrix4x4 二項演算子
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

// ============================================
// クォータニオン定義・関連関数
// ============================================
struct Quaternion {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
};

// クォータニオン演算関数
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs); // Quaternionの積
Quaternion IdentityQuaternion();                                   // 乗法単位を返す
Quaternion Conjugate(const Quaternion& quaternion);                // 共役Quaternionを返す
float Norm(const Quaternion& quaternion);                          // Quaternionのnormを返す
Quaternion Normalize(const Quaternion& quaternion);                // 正規化したQuaternionを返す
Quaternion Inverse(const Quaternion& quaternion);                  // 逆Quaternionを返す

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle); // 任意軸回転を表すQuaternionの生成 
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);  // ベクトルをQuaternionで回転させた結果のベクトルを求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);                   // Quaternionから回転行列を求める

class MyMath {
public:
	// ベクトル演算
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	static Vector3 Subtract(const Vector3& a, const Vector3& b);
	static float Length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Cross(const Vector3& a, const Vector3& b);
	static float Dot(const Vector3& v1, const Vector3& v2);

	// 座標変換ヘルパー
	static Vector3 ToCartesian(const Spherical& s);
	static Vector3 ToCartesian(const Cylindrical& c);
	static Vector3 ToCartesian(const Polar2D& p);

	// 補間・幾何計算
	static Vector3 GetBarycentricPosition(const Vector3& a, const Vector3& b, const Vector3& c, float u, float v, float w);
	static Vector3 Project(const Vector3& v1, const Vector3& v2);
	static Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	// 行列演算
	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	static Matrix4x4 Inverse(const Matrix4x4& m);
	static Matrix4x4 Transpose(const Matrix4x4& m);
	static Matrix4x4 Identity();

	// 変換行列生成
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	// 投影・ビュー行列生成
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	static Matrix4x4 MakeCameraMatrix(const Spherical& spherical);

	// 衝突判定
	static bool IsCollision(const Sphere& s1, const Sphere& s2);
	static bool IsCollision(const Sphere& sphere, const Plane& plane);
	static bool IsCollision(const Segment& segment, const Plane& plane);
	static bool IsCollision(const Triangle& triangle, const Segment& segment);
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);
	static bool IsCollision(const AABB& aabb, const Segment& segment);

	// 曲線描画
	static void DrawBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
};
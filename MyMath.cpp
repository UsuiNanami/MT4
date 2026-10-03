#define NOMINMAX
#define _USE_MATH_DEFINES
#include "MyMath.h"
#include <Novice.h>
#include <cassert>
#include <cmath>

// ============================================
// ベクトル基本演算
// ============================================ 
Vector3 MyMath::Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

Vector3 MyMath::Subtract(const Vector3& a, const Vector3& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

float MyMath::Length(const Vector3& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

Vector3 MyMath::Normalize(const Vector3& v) {
	float len = Length(v);
	if (len < 1e-6f)
		return {0.0f, 0.0f, 0.0f};
	return {v.x / len, v.y / len, v.z / len};
}

Vector3 MyMath::Cross(const Vector3& a, const Vector3& b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

float MyMath::Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

// ============================================
// 座標変換関数
// ============================================
Vector3 MyMath::ToCartesian(const Spherical& s) {
	float cosTheta = std::cos(s.theta);
	return {s.radius * cosTheta * std::cos(s.phi), s.radius * std::sin(s.theta), s.radius * cosTheta * std::sin(s.phi)};
}

Vector3 MyMath::ToCartesian(const Cylindrical& c) { return {c.radius * std::cos(c.theta), c.height, c.radius * std::sin(c.theta)}; }

Vector3 MyMath::ToCartesian(const Polar2D& p) { return {p.radius * std::cos(p.theta), p.radius * std::sin(p.theta), 0.0f}; }

Vector3 MyMath::GetBarycentricPosition(const Vector3& a, const Vector3& b, const Vector3& c, float u, float v, float w) {
	return {u * a.x + v * b.x + w * c.x, u * a.y + v * b.y + w * c.y, u * a.z + v * b.z + w * c.z};
}

Vector3 MyMath::Project(const Vector3& v1, const Vector3& v2) {
	float dot = Dot(v1, v2);
	float lengthSquared = Dot(v2, v2);
	assert(lengthSquared != 0.0f);
	float scale = dot / lengthSquared;
	return {scale * v2.x, scale * v2.y, scale * v2.z};
}

Vector3 MyMath::ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 segmentStart = segment.origin;
	Vector3 segmentEnd = {segment.origin.x + segment.diff.x, segment.origin.y + segment.diff.y, segment.origin.z + segment.diff.z};
	Vector3 segmentVector = Subtract(segmentEnd, segmentStart);
	Vector3 pointVector = Subtract(point, segmentStart);
	float dot = Dot(pointVector, segmentVector);
	float lengthSquared = Dot(segmentVector, segmentVector);
	assert(lengthSquared != 0.0f);
	float t = std::clamp(dot / lengthSquared, 0.0f, 1.0f);
	return {segmentStart.x + t * segmentVector.x, segmentStart.y + t * segmentVector.y, segmentStart.z + t * segmentVector.z};
}

Vector3 MyMath::Lerp(const Vector3& v1, const Vector3& v2, float t) { return {v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t}; }

// ============================================
// 行列演算
// ============================================
// 4x4行列の加算
Matrix4x4 MyMath::Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

// 4x4行列の減算
Matrix4x4 MyMath::Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

// 4x4行列の掛け算
Matrix4x4 MyMath::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

// 3x3行列の行列式を計算する関数
static float Determinant3x3(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33) {
	return a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32;
}

// 4x4行列の逆行列を計算する関数
Matrix4x4 MyMath::Inverse(const Matrix4x4& m) {
	float det = m.m[0][0] * Determinant3x3(m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]) -
	            m.m[0][1] * Determinant3x3(m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]) +
	            m.m[0][2] * Determinant3x3(m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]) -
	            m.m[0][3] * Determinant3x3(m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]);

	if (std::abs(det) < 1.0e-6f)
		return {};

	float invDet = 1.0f / det;
	Matrix4x4 result;

	result.m[0][0] = Determinant3x3(m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]) * invDet;
	result.m[0][1] = -Determinant3x3(m.m[0][1], m.m[0][2], m.m[0][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]) * invDet;
	result.m[0][2] = Determinant3x3(m.m[0][1], m.m[0][2], m.m[0][3], m.m[1][1], m.m[1][2], m.m[1][3], m.m[3][1], m.m[3][2], m.m[3][3]) * invDet;
	result.m[0][3] = -Determinant3x3(m.m[0][1], m.m[0][2], m.m[0][3], m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3]) * invDet;

	result.m[1][0] = -Determinant3x3(m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]) * invDet;
	result.m[1][1] = Determinant3x3(m.m[0][0], m.m[0][2], m.m[0][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]) * invDet;
	result.m[1][2] = -Determinant3x3(m.m[0][0], m.m[0][2], m.m[0][3], m.m[1][0], m.m[1][2], m.m[1][3], m.m[3][0], m.m[3][2], m.m[3][3]) * invDet;
	result.m[1][3] = Determinant3x3(m.m[0][0], m.m[0][2], m.m[0][3], m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3]) * invDet;

	result.m[2][0] = Determinant3x3(m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]) * invDet;
	result.m[2][1] = -Determinant3x3(m.m[0][0], m.m[0][1], m.m[0][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]) * invDet;
	result.m[2][2] = Determinant3x3(m.m[0][0], m.m[0][1], m.m[0][3], m.m[1][0], m.m[1][1], m.m[1][3], m.m[3][0], m.m[3][1], m.m[3][3]) * invDet;
	result.m[2][3] = -Determinant3x3(m.m[0][0], m.m[0][1], m.m[0][3], m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][2]) * invDet;

	result.m[3][0] = -Determinant3x3(m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]) * invDet;
	result.m[3][1] = Determinant3x3(m.m[0][0], m.m[0][1], m.m[0][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]) * invDet;
	result.m[3][2] = -Determinant3x3(m.m[0][0], m.m[0][1], m.m[0][2], m.m[1][0], m.m[1][1], m.m[1][2], m.m[3][0], m.m[3][1], m.m[3][2]) * invDet;
	result.m[3][3] = Determinant3x3(m.m[0][0], m.m[0][1], m.m[0][2], m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2]) * invDet;

	return result;
}

// 4x4行列の転置
Matrix4x4 MyMath::Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

// 4x4単位行列の生成
Matrix4x4 MyMath::Identity() {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		result.m[i][i] = 1.0f;
	}
	return result;
}

// ============================================
// 変換行列・カメラ行列
// ============================================
// 平行移動行列の生成
Matrix4x4 MyMath::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = Identity();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}

// 拡大縮小行列の生成
Matrix4x4 MyMath::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;
	return result;
}

// 4x4行列によるベクトルの変換
Vector3 MyMath::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

// X軸回転行列の生成
Matrix4x4 MyMath::MakeRotateXMatrix(float radian) {
	Matrix4x4 result = Identity();
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	result.m[1][1] = cosTheta;
	result.m[1][2] = sinTheta;
	result.m[2][1] = -sinTheta;
	result.m[2][2] = cosTheta;
	return result;
}

// Y軸回転行列の生成
Matrix4x4 MyMath::MakeRotateYMatrix(float radian) {
	Matrix4x4 result = Identity();
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	result.m[0][0] = cosTheta;
	result.m[0][2] = -sinTheta;
	result.m[2][0] = sinTheta;
	result.m[2][2] = cosTheta;
	return result;
}

// Z軸回転行列の生成
Matrix4x4 MyMath::MakeRotateZMatrix(float radian) {
	Matrix4x4 result = Identity();
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	result.m[0][0] = cosTheta;
	result.m[0][1] = sinTheta;
	result.m[1][0] = -sinTheta;
	result.m[1][1] = cosTheta;
	return result;
}

// アフィン変換行列の生成
Matrix4x4 MyMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);
	return Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
}

// ============================================
// 投影・ビュー行列生成
// ============================================
Matrix4x4 MyMath::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};
	float f = 1.0f / std::tan(fovY / 2.0f);
	result.m[0][0] = f / aspectRatio;
	result.m[1][1] = f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	return result;
}

// 透視投影行列の生成
Matrix4x4 MyMath::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {};
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1.0f;
	return result;
}

// ビューポート変換行列の生成
Matrix4x4 MyMath::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;
	return result;
}

// カメラ行列の生成
Matrix4x4 MyMath::MakeCameraMatrix(const Spherical& sphericalInput) {
	Spherical s = sphericalInput;
	const float limit = std::numbers::pi_v<float> / 2.0f - 0.001f;
	s.radius = std::max(s.radius, 0.001f);
	s.theta = std::clamp(s.theta, -limit, limit);

	Vector3 eye = ToCartesian(s);
	Vector3 target = {0.0f, 0.0f, 0.0f};

	Vector3 forward = Normalize(Subtract(target, eye));
	Vector3 worldUp = {0.0f, 1.0f, 0.0f};
	Vector3 right = Normalize(Cross(worldUp, forward));
	Vector3 up = Cross(forward, right);

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

// ============================================
// 衝突判定
// ============================================
// 球と球の衝突判定
bool MyMath::IsCollision(const Sphere& s1, const Sphere& s2) {
	float distance = Length(Subtract(s2.center, s1.center));
	return distance <= (s1.radius + s2.radius);
}

// 球と平面の衝突判定
bool MyMath::IsCollision(const Sphere& sphere, const Plane& plane) {
	float t = Dot(plane.normal, sphere.center) - plane.distance;
	return std::abs(t) <= sphere.radius;
}

// 線分と平面の衝突判定
bool MyMath::IsCollision(const Segment& segment, const Plane& plane) {
	float dot = Dot(plane.normal, segment.diff);
	if (dot == 0.0f)
		return false;
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
	return (0.0f <= t && t <= 1.0f);
}

// 三角形と線分の衝突判定
bool MyMath::IsCollision(const Triangle& triangle, const Segment& segment) {
	Vector3 p0 = triangle.vertices[0];
	Vector3 p1 = triangle.vertices[1];
	Vector3 p2 = triangle.vertices[2];

	Vector3 v01 = Subtract(p1, p0);
	Vector3 v02 = Subtract(p2, p0);

	Vector3 normal = Cross(v01, v02);
	float len = Length(normal);
	if (len == 0.0f)
		return false;
	normal = {normal.x / len, normal.y / len, normal.z / len};

	float d = Dot(normal, p0);
	float dot = Dot(normal, segment.diff);
	if (dot == 0.0f)
		return false;

	float t = (d - Dot(segment.origin, normal)) / dot;
	if (t < 0.0f || t > 1.0f)
		return false;

	Vector3 P = {segment.origin.x + segment.diff.x * t, segment.origin.y + segment.diff.y * t, segment.origin.z + segment.diff.z * t};

	Vector3 v12 = Subtract(p2, p1);
	Vector3 v20 = Subtract(p0, p2);

	Vector3 v1p = Subtract(P, p1);
	Vector3 v2p = Subtract(P, p2);
	Vector3 v0p = Subtract(P, p0);

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	return (Dot(cross01, normal) >= 0 && Dot(cross12, normal) >= 0 && Dot(cross20, normal) >= 0);
}

// 軸平行境界ボックス同士の衝突判定
bool MyMath::IsCollision(const AABB& a, const AABB& b) {
	if (a.min.x > b.max.x || a.max.x < b.min.x)
		return false;
	if (a.min.y > b.max.y || a.max.y < b.min.y)
		return false;
	if (a.min.z > b.max.z || a.max.z < b.min.z)
		return false;
	return true;
}

// 軸平行境界ボックスと球の衝突判定
bool MyMath::IsCollision(const AABB& aabb, const Sphere& sphere) {
	Vector3 closestPoint{std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)};
	float distance = Length(Subtract(closestPoint, sphere.center));
	return distance <= sphere.radius;
}

// 軸平行境界ボックスと線分の衝突判定
bool MyMath::IsCollision(const AABB& aabb, const Segment& segment) {
	float t_min_x, t_max_x;
	float t_min_y, t_max_y;
	float t_min_z, t_max_z;

	if (segment.diff.x == 0.0f) {
		if (segment.origin.x < aabb.min.x || segment.origin.x > aabb.max.x)
			return false;
		t_min_x = -std::numeric_limits<float>::infinity();
		t_max_x = std::numeric_limits<float>::infinity();
	} else {
		t_min_x = (aabb.min.x - segment.origin.x) / segment.diff.x;
		t_max_x = (aabb.max.x - segment.origin.x) / segment.diff.x;
		if (t_min_x > t_max_x)
			std::swap(t_min_x, t_max_x);
	}

	if (segment.diff.y == 0.0f) {
		if (segment.origin.y < aabb.min.y || segment.origin.y > aabb.max.y)
			return false;
		t_min_y = -std::numeric_limits<float>::infinity();
		t_max_y = std::numeric_limits<float>::infinity();
	} else {
		t_min_y = (aabb.min.y - segment.origin.y) / segment.diff.y;
		t_max_y = (aabb.max.y - segment.origin.y) / segment.diff.y;
		if (t_min_y > t_max_y)
			std::swap(t_min_y, t_max_y);
	}

	if (segment.diff.z == 0.0f) {
		if (segment.origin.z < aabb.min.z || segment.origin.z > aabb.max.z)
			return false;
		t_min_z = -std::numeric_limits<float>::infinity();
		t_max_z = std::numeric_limits<float>::infinity();
	} else {
		t_min_z = (aabb.min.z - segment.origin.z) / segment.diff.z;
		t_max_z = (aabb.max.z - segment.origin.z) / segment.diff.z;
		if (t_min_z > t_max_z)
			std::swap(t_min_z, t_max_z);
	}

	float t_min = (std::max)((std::max)(t_min_x, t_min_y), t_min_z);
	float t_max = (std::min)((std::min)(t_max_x, t_max_y), t_max_z);

	return (t_min <= t_max && t_max >= 0.0f && t_min <= 1.0f);
}

// ============================================
// 描画関数実装
// ============================================
// 
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 16;
	const float kLonEvery = 2.0f * static_cast<float>(M_PI) / kSubdivision;
	const float kLatEvery = static_cast<float>(M_PI) / kSubdivision;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * static_cast<float>(latIndex);
		float nextLat = lat + kLatEvery;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = static_cast<float>(lonIndex) * kLonEvery;
			float nextLon = lon + kLonEvery;

			Vector3 a = {
			    sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon), sphere.center.y + sphere.radius * std::sin(lat), sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon)};
			Vector3 b = {
			    sphere.center.x + sphere.radius * std::cos(nextLat) * std::cos(lon), sphere.center.y + sphere.radius * std::sin(nextLat),
			    sphere.center.z + sphere.radius * std::cos(nextLat) * std::sin(lon)};
			Vector3 c = {
			    sphere.center.x + sphere.radius * std::cos(lat) * std::cos(nextLon), sphere.center.y + sphere.radius * std::sin(lat),
			    sphere.center.z + sphere.radius * std::cos(lat) * std::sin(nextLon)};

			Vector3 aScreen = MyMath::Transform(MyMath::Transform(a, viewProjectionMatrix), viewportMatrix);
			Vector3 bScreen = MyMath::Transform(MyMath::Transform(b, viewProjectionMatrix), viewportMatrix);
			Vector3 cScreen = MyMath::Transform(MyMath::Transform(c, viewProjectionMatrix), viewportMatrix);

			Novice::DrawLine(int(aScreen.x), int(aScreen.y), int(bScreen.x), int(bScreen.y), color);
			Novice::DrawLine(int(aScreen.x), int(aScreen.y), int(cScreen.x), int(cScreen.y), color);
		}
	}
}

// グリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHalfWidth + (float(xIndex) * kGridEvery);
		Vector3 startWorld = {x, 0.0f, -kGridHalfWidth};
		Vector3 endWorld = {x, 0.0f, kGridHalfWidth};

		Vector3 startScreen = MyMath::Transform(MyMath::Transform(startWorld, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = MyMath::Transform(MyMath::Transform(endWorld, viewProjectionMatrix), viewportMatrix);
		uint32_t color = (std::abs(x) < 0.001f) ? 0x000000FF : 0xAAAAAAFF;

		Novice::DrawLine(int(startScreen.x), int(startScreen.y), int(endScreen.x), int(endScreen.y), color);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHalfWidth + (float(zIndex) * kGridEvery);
		Vector3 startWorld = {-kGridHalfWidth, 0.0f, z};
		Vector3 endWorld = {kGridHalfWidth, 0.0f, z};

		Vector3 startScreen = MyMath::Transform(MyMath::Transform(startWorld, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = MyMath::Transform(MyMath::Transform(endWorld, viewProjectionMatrix), viewportMatrix);
		uint32_t color = (std::abs(z) < 0.001f) ? 0x000000FF : 0xAAAAAAFF;

		Novice::DrawLine(int(startScreen.x), int(startScreen.y), int(endScreen.x), int(endScreen.y), color);
	}
}

// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = {plane.normal.x * plane.distance, plane.normal.y * plane.distance, plane.normal.z * plane.distance};

	Vector3 perpendiculars[4];
	perpendiculars[0] = MyMath::Cross(plane.normal, {0.0f, 1.0f, 0.0f});
	if (MyMath::Length(perpendiculars[0]) < 0.001f) {
		perpendiculars[0] = MyMath::Cross(plane.normal, {1.0f, 0.0f, 0.0f});
	}
	float len0 = MyMath::Length(perpendiculars[0]);
	perpendiculars[0] = {perpendiculars[0].x / len0, perpendiculars[0].y / len0, perpendiculars[0].z / len0};

	perpendiculars[1] = MyMath::Cross(plane.normal, perpendiculars[0]);
	float len1 = MyMath::Length(perpendiculars[1]);
	perpendiculars[1] = {perpendiculars[1].x / len1, perpendiculars[1].y / len1, perpendiculars[1].z / len1};

	perpendiculars[2] = {-perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z};
	perpendiculars[3] = {-perpendiculars[1].x, -perpendiculars[1].y, -perpendiculars[1].z};

	Vector3 points[4];
	for (int i = 0; i < 4; ++i) {
		Vector3 extend = {perpendiculars[i].x * 2.0f, perpendiculars[i].y * 2.0f, perpendiculars[i].z * 2.0f};
		Vector3 point = MyMath::Add(center, extend);
		points[i] = MyMath::Transform(MyMath::Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 4; ++i) {
		int next = (i + 1) % 4;
		Novice::DrawLine(int(points[i].x), int(points[i].y), int(points[next].x), int(points[next].y), color);
	}
}

// 線分の描画
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = segment.origin;
	Vector3 end = {segment.origin.x + segment.diff.x, segment.origin.y + segment.diff.y, segment.origin.z + segment.diff.z};

	Vector3 startScreen = MyMath::Transform(MyMath::Transform(start, viewProjectionMatrix), viewportMatrix);
	Vector3 endScreen = MyMath::Transform(MyMath::Transform(end, viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine(int(startScreen.x), int(startScreen.y), int(endScreen.x), int(endScreen.y), color);
}

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 p0 = MyMath::Transform(MyMath::Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	Vector3 p1 = MyMath::Transform(MyMath::Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	Vector3 p2 = MyMath::Transform(MyMath::Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawTriangle(int(p0.x), int(p0.y), int(p1.x), int(p1.y), int(p2.x), int(p2.y), color, kFillModeWireFrame);
}

// 軸平行境界ボックスの描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 vertices[8] = {
	    {aabb.min.x, aabb.min.y, aabb.min.z},
        {aabb.max.x, aabb.min.y, aabb.min.z},
        {aabb.max.x, aabb.max.y, aabb.min.z},
        {aabb.min.x, aabb.max.y, aabb.min.z},
	    {aabb.min.x, aabb.min.y, aabb.max.z},
        {aabb.max.x, aabb.min.y, aabb.max.z},
        {aabb.max.x, aabb.max.y, aabb.max.z},
        {aabb.min.x, aabb.max.y, aabb.max.z},
	};

	for (int i = 0; i < 8; ++i) {
		vertices[i] = MyMath::Transform(MyMath::Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	int edges[12][2] = {
	    {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };

	for (int i = 0; i < 12; ++i) {
		Novice::DrawLine(int(vertices[edges[i][0]].x), int(vertices[edges[i][0]].y), int(vertices[edges[i][1]].x), int(vertices[edges[i][1]].y), color);
	}
}

// ベジェ曲線の描画
void MyMath::DrawBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const int kSegmentCount = 32;

	for (int i = 0; i < kSegmentCount; ++i) {
		float t0 = float(i) / kSegmentCount;
		float t1 = float(i + 1) / kSegmentCount;

		Vector3 a = Lerp(Lerp(p0, p1, t0), Lerp(p1, p2, t0), t0);
		Vector3 b = Lerp(Lerp(p0, p1, t1), Lerp(p1, p2, t1), t1);

		Vector3 aScreen = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
		Vector3 bScreen = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine((int)aScreen.x, (int)aScreen.y, (int)bScreen.x, (int)bScreen.y, color);

		Vector3 p0Screen = Transform(Transform(p0, viewProjectionMatrix), viewportMatrix);
		Vector3 p1Screen = Transform(Transform(p1, viewProjectionMatrix), viewportMatrix);
		Vector3 p2Screen = Transform(Transform(p2, viewProjectionMatrix), viewportMatrix);

		Novice::DrawEllipse((int)p0Screen.x, (int)p0Screen.y, 8, 8, 0.0f, BLACK, kFillModeSolid);
		Novice::DrawEllipse((int)p1Screen.x, (int)p1Screen.y, 8, 8, 0.0f, BLACK, kFillModeSolid);
		Novice::DrawEllipse((int)p2Screen.x, (int)p2Screen.y, 8, 8, 0.0f, BLACK, kFillModeSolid);
	}
}

// ============================================
// 演算子オーバーロード実装
// ============================================
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return MyMath::Add(v1, v2); } // Vector3同士の加算
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return MyMath::Subtract(v1, v2); } // Vector3同士の減算
Vector3 operator*(float s, const Vector3& v) { return {v.x * s, v.y * s, v.z * s}; }         // スカラーとVector3の乗算
Vector3 operator*(const Vector3& v, float s) { return {v.x * s, v.y * s, v.z * s}; }         // Vector3とスカラーの乗算
Vector3 operator/(const Vector3& v, float s) { return {v.x / s, v.y / s, v.z / s}; }         // Vector3とスカラーの除算

Vector3 operator-(const Vector3& v) { return {-v.x, -v.y, -v.z}; } // Vector3の符号反転
Vector3 operator+(const Vector3& v) { return v; }                  // Vector3の単項プラス

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return MyMath::Add(m1, m2); } // Matrix4x4同士の加算
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return MyMath::Subtract(m1, m2); } // Matrix4x4同士の減算
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return MyMath::Multiply(m1, m2); } // Matrix4x4同士の掛け算


// ============================================
// クォータニオン演算関数実装
// ============================================
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	return {
	    lhs.x * rhs.w + lhs.w * rhs.x + lhs.y * rhs.z - lhs.z * rhs.y, lhs.y * rhs.w + lhs.w * rhs.y + lhs.z * rhs.x - lhs.x * rhs.z, lhs.z * rhs.w + lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x,
	    lhs.w * rhs.w - (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z)};
}

Quaternion IdentityQuaternion() { return {0.0f, 0.0f, 0.0f, 1.0f}; }

Quaternion Conjugate(const Quaternion& quaternion) { return {-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w}; }

float Norm(const Quaternion& quaternion) { return std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w); }

Quaternion Normalize(const Quaternion& quaternion) {
	float norm = Norm(quaternion);
	if (norm <= 0.0f) {
		return {0.0f, 0.0f, 0.0f, 0.0f};
	}
	return {quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm};
}

Quaternion Inverse(const Quaternion& quaternion) {
	float norm = Norm(quaternion);
	float normSq = norm * norm;
	if (normSq <= 0.0f) {
		return {0.0f, 0.0f, 0.0f, 0.0f};
	}
	Quaternion conj = Conjugate(quaternion);
	return {conj.x / normSq, conj.y / normSq, conj.z / normSq, conj.w / normSq};
}
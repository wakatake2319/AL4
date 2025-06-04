#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

// 左右の振り向き
enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_ = {};

	// ================================
	// 移動
	// ================================
	// 慣性移動
	static inline const float kAcceleration = 0.01f;
	// 減速
	static inline const float kAtteleration = 0.05f;
	// 最大速度
	static inline const float kLimitRunSpeed = 0.3f;


	// ===============================
	// 振り向き
	// ===============================
	// 角度補強
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 0.3f;
	// 左右の振り向き
	LRDirection lrDirection_ = LRDirection::kRight;


	// ===============================
	// 着地
	// ===============================
	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;


	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// カメラ
	Camera* camera_;
};
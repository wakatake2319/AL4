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

	// 慣性移動
	static inline const float kAcceleration = 0.01f;

	// 減速
	static inline const float kAtteleration = 0.05f;

	// 最大速度
	static inline const float kLimitRunSpeed = 0.3f;

	// 左右の振り向き
	LRDirection lrDirection_ = LRDirection::kRight;

	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// カメラ
	Camera* camera_;
};
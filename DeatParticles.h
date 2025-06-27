#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include <array>
#include <numbers>
#include <algorithm>

class DeathParticles {
public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	bool IsFinished() { return isFinished_; }

private:
	// パーティクルの数
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransform_;

	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;

	// ====================================
	// パーティクルの移動関連の定数
	// ====================================
	// パーティクルの移動速度
	static inline const float kSpeed = 0.1f;
	// パーティクルの持続時間(削除までの時間)(秒)
	static inline const float kDuration = 2.0f; 
	// 分割した一個分の角度
	static inline const float kAngleUint = 2.0f * std::numbers::pi_v<float> / kNumParticles; 


	// ====================================
	// パーティクルの削除処理
	// ====================================
	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_ = 0.0f;

	// ====================================
	// フェードアウト処理
	// ====================================
	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;

};
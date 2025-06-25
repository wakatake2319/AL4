#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include <array>

class DeathParticles {
public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// パーティクルの数
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransform_;

	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;


};
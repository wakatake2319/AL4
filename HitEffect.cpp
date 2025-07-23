#include "HitEffect.h"

Model* HitEffect::model_ = nullptr;
Camera* HitEffect::camera_ = nullptr;

// インスタンスの生成と初期化
HitEffect* HitEffect::Create(const Vector3& position) {
	// インスタンスの生成
	HitEffect* instance = new HitEffect();
	// newの失敗を検出
	assert(instance);
	// インスタンスの初期化
	instance->Initialize(position);
	// 初期化したインスタンスを返す
	return instance;


}

void HitEffect::Initialize(const KamataEngine::Vector3& position) {
	// 円形エフェクト
	circleWorldTransform_.translation_ = position;
	circleWorldTransform_.Initialize();
	circleWorldTransform_.translation_.z = -1.0f;
	objectColor_.Initialize();

}
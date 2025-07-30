#include <cassert>
#include <random>
#include <numbers>
#include "HitEffect.h"
#include "Math.h"

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
	std::random_device seedGenerator;
	std::mt19937_64 randomEngine;
	randomEngine.seed(seedGenerator());
	std::uniform_real_distribution<float> rotationDistribution(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);

	// 楕円エフェクト
	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		worldTransform.scale_ = {0.0f, 0.0f, 1.0f};
		worldTransform.rotation_.y = rotationDistribution(randomEngine);
		worldTransform.Initialize();
		worldTransform.translation_.z = -1.0f;
	}


	// 円形エフェクト
	circleWorldTransform_.translation_ = position;
	circleWorldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	circleWorldTransform_.Initialize();
	circleWorldTransform_.translation_.z = -1.0f;
	objectColor_.Initialize();

}


// 更新
void HitEffect::Update() {
	// 円のワールドトランスフォームを更新
	WorldTransformUpdate(circleWorldTransform_);
}

// 描画
void HitEffect::Draw() {
	assert(model_);
	assert(camera_);

	model_->Draw(circleWorldTransform_, *camera_, &objectColor_);
	OutputDebugStringA("Draw!\n");
}
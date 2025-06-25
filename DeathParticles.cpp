#include "DeatParticles.h"

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// パーティクルの初期化
	for (WorldTransform& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position; // 全て同じ位置からスタート
	}
}

void DeathParticles::Update() {
	for (auto& worldTransform : worldTransform_) {
		// ワールド行列更新（アフィン変換～DirectXに転送）
		WorldTransformUpdate(worldTransform);
	}
}

void DeathParticles::Draw() {
	for (auto& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera_);
	}
}
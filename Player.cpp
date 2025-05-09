#include "Player.h"

using namespace KamataEngine;
// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	assert(model);

	model_ = model; 

	textureHandle_ = textureHandle;

	camera_ = camera;

	worldTransform_.Initialize();
}

// 更新
void Player::Update() { worldTransform_.TransferMatrix(); }

// 描画
void Player::Draw() { model_->Draw(worldTransform_, *camera_, textureHandle_); }
#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {

	assert(model);
	// モデル
	model_ = model;
	// テクスチャハンドル
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	camera_ = camera;
}

void Player ::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player ::Draw() {
	// モデル描画
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}
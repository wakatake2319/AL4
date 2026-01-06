#include "wakame.h"

// 初期化
void wakame::Initialize(Model* model, Camera* camera, const Vector3& position) {
	worldTransform_.Initialize();

	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	basePosition_ = position; // 基準位置を保存
}

// 更新
void wakame::Update() { WorldTransformUpdate(worldTransform_); }

// 描画
void wakame::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}

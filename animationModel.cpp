#include "animationModel.h"
#include <cassert>


void animationModel::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	// glTFモデルをロード
	// ※ Resources/models/player/player.gltf など
	//model_.reset(Model::LoadFromGLTF("Resources/models/player/player.gltf"));

	// ワールド変換初期化
	worldTransform_.Initialize();

	// 出現位置
	worldTransform_.translation_ = position;

	// サイズ調整（glTFは小さいことが多い）
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

}

void animationModel::Update() {
	// ワールド行列更新
	WorldTransformUpdate(worldTransform_);
}

void animationModel::Draw() {
	if (model_) {
		model_->Draw(worldTransform_, *camera_);
	}
}
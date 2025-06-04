#include "Skydome.h"

// 初期化
void Skydome::Initialize(Model* model, Camera* camera) {
	worldTransform_.Initialize();

	model_ = model; 
	camera_ = camera;
}


// 更新
void Skydome::Update() {
	worldTransform_.TransferMatrix(); 
}


// 描画
void Skydome::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);

}

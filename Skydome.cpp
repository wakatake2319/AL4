#include "Skydome.h"

// 初期化
void Skydome::Initialize() {}

// 更新
void Skydome::Update() {}

// 描画
void Skydome::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 3Dモデル描画
	model_->Draw(worldTransform_, camera_);

	Model::PostDraw();
}

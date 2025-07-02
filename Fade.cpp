#include <algorithm>
#include "Fade.h"

void Fade::Initialize() {
	// スプライトの初期化
	sprite_ = new Sprite();
	sprite_->SetSize(Vector2(1280,720.0f));
	sprite_->SetColor(Vector4(0, 0, 0, 1.0f));

}

void Fade::Update() {

}

void Fade::Draw() {
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	// スプライトの描画
	sprite_->Draw();
	Sprite::PostDraw();
}
#include <algorithm>
#include "Fade.h"

void Fade::Initialize() {
	// スプライトの初期化
	sprite_ = Sprite::Create(0, Vector2{});
	sprite_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

}

void Fade::Update() {
	switch (status_) {

	// フェード無し
	case Status::None:
		break;

	// フェードイン
	case Status::FadeIn:
		// 1フレームあたりの秒数をカウントアップ
		counter_ += 1.0f / 60.0f; 
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどα値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f - counter_ / duration_, 0.0f, 1.0f)));
		break;

	// フェードアウト
	case Status::FadeOut:
		// 1フレームあたりの秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどα値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	}
}

void Fade::Draw() {
	if (status_ == Status::None) {
		// フェードが無い場合は描画しない
		return;
	}
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	// スプライトの描画
	sprite_->Draw();
	Sprite::PostDraw();
}

// フェード開始
void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

// フェード停止
void Fade::Stop() {
	status_ = Status::None;

}

// フェード終了判定
bool Fade::IsFinished() const {
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:
		if (counter_ >= duration_) { 
			// フェードが終了した
			return true;
		} else {
			// フェードがまだ続いている
			return false; 
		}
	}
	// フェードが無い場合は終了とみなす
	return true; 
}
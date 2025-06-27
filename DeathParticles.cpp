#include "DeatParticles.h"

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// パーティクルの初期化
	for (WorldTransform& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position; // 全て同じ位置からスタート
	}

	// フェードアウト処理
	objectColor_.Initialize();
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
}

void DeathParticles::Update() {
	// 終了なら何も表示しない
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransform_) {
		// ワールド行列更新（アフィン変換～DirectXに転送）
		WorldTransformUpdate(worldTransform);
	}

	// ===========================
	// 移動
	// ===========================
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {kSpeed, 0.0f, 0.0f};
		// パーティクルの角度を計算
		float angle = kAngleUint * i;
		// Z軸周りの回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// パーティクルの位置を更新
		worldTransform_[i].translation_ += velocity;
	}

	// ===========================
	// 削除処理
	// ===========================
	// カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;
	// 持続時間の上限に達した時の処理
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}

	// ===========================
	// フェードアウト処理
	// ===========================
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
}

void DeathParticles::Draw() {
	// 終了なら何も表示しない
	if (isFinished_) {
		return;
	}
	// パーティクル描画
	for (auto& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera_, &objectColor_);
	}

}
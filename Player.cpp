#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	camera_ = camera;
}

void Player::Update() {

	// 慣性移動
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 左移動中の右入力
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAtteleration);
			}

			// 移動
			acceleration.x += kAcceleration;

			// 左右状態切り替え
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
			}

		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 右移動中の左入力
			if (velocity_.x > 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAtteleration);
			}
			// 移動
			acceleration.x -= kAcceleration;

			// 左右状態切り替え
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
			}
		}

		// 加速/減速
		velocity_ += acceleration;

		// 最大速度の制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		// 移動入力をしてない場合は減衰させる
		velocity_.x *= (1.0f - kAtteleration);
	}

	// 移動
	worldTransform_.translation_ += velocity_;

	// 旋回制御
	float destinationRotationYTable[] = {
	    std::numbers::pi_v<float> / 2.0f,
	    std::numbers::pi_v<float> * 3.0f / 2.0f
	};

	float destinationRotationY = destinationRotationYTable[static_cast<int>(lrDirection_)];

	worldTransform_.rotation_.y = destinationRotationY;

	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() {

	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}
#include "Player.h"
#include <cassert>
#include <numbers>
#include <algorithm>

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;



}


void Player::Update() {

	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
	
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

					acceleration.x -= kAcceleration;
		}
	
		// 加速/減速
		// オーバーロードの関数を作る
		velocity_ += acceleration;
	}


	// 移動
	worldTransform_.translation_ += velocity_;



	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() {

	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}
#include "Enemy.h"
#include <numbers>
#include "Math.h"

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 速度設定
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}
void Enemy::Update()
{
	// 移動
	worldTransform_.translation_ += velocity_;

	// タイマー加算
	walkTimer_ += 1.0f / 60.0f;

	// 回転アニメーション
	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = PI/180*(degree);


	// ワールド行列更新
	WorldTransformUpdate(worldTransform_);
}
void Enemy::Draw()
{
	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}

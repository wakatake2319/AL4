#include "Enemy.h"
#include <numbers>
#include "Math.h"
#include "Player.h"

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 初期回転
	baseRotationY_ = std::numbers::pi_v<float> * 3.0f / 2.0f;
	walkAnimRotationY_ = 0.0f;
	worldTransform_.rotation_.y = baseRotationY_;
	// 速度設定
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;


}
void Enemy::Update()
{

	// =========================
	// Behavior遷移の実装
	// =========================
	if (behaviorRequest_ != Behavior::kUnknown) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_;

		// 各振る舞いごとの初期化を実行
		switch (behavior_) {

		// 通常行動
		case Behavior::kDeath:
		default:
			counter_ = 0;
			break;
		}
		// 振る舞いリクエストを初期化
		behaviorRequest_ = Behavior::kUnknown;
	}


	switch (behavior_) {
	case Behavior::kRoot: {

		if (walkTimer_ >= 500.0f) {
			walkTimer_ = 0.0f;
			velocity_ *= -1.0f;

			// 反転後の向きに合わせて回転を設定する
			if (velocity_.x > 0.0f) {
				// 右向き
				baseRotationY_ = std::numbers::pi_v<float> / 2.0f;
			} else {
				// 左向き
				baseRotationY_ = std::numbers::pi_v<float> * 3.0f / 2.0f;
			}
		}

		// 移動
		worldTransform_.translation_ += velocity_;

		// 歩行アニメーション
		const float kWalkSwing = ToRadians(5.0f);

		walkAnimRotationY_ = sinf(walkTimer_ * 0.1f) * kWalkSwing;

		worldTransform_.rotation_.y = baseRotationY_ + walkAnimRotationY_;

		// タイマー加算
		walkTimer_++;

		WorldTransformUpdate(worldTransform_);
		break;
	}

		// デス演出
	case Behavior::kDeath: {

		// デス演出のカウンターを進める
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);

		// ワールド行列更新
		WorldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isDeath_ = true;
		}

		break;
	}
	}

}
void Enemy::Draw()
{
	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}


Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { 

	// デスフラグを立てる
	isDeath_ = true;
	if (behavior_ == Behavior::kDeath) {
		// 敵がやられているなら何もしない
		return;
	}
	// プレイヤーが攻撃中なら敵が死ぬ
	if (player->IsAttack()) {

		// 衝突無効化
		behaviorRequest_ = Behavior::kDeath;
	} 



}

void Enemy::OnHitByAttack() {

	if (behavior_ == Behavior::kDeath) {
		return;
	}

	behaviorRequest_ = Behavior::kDeath;
}
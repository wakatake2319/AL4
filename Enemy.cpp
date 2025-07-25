#include "Enemy.h"
#include <numbers>
#include <cassert>
#include "Math.h"
#include "Player.h"
#include "GameScene.h"

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
	case Behavior::kRoot:

		// 移動
		worldTransform_.translation_ += velocity_;

		// タイマー加算
		walkTimer_ += 1.0f / 60.0f;

		// 回転アニメーション
		worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime); // ワールド行列更新
		WorldTransformUpdate(worldTransform_);
		break;

		// デス演出
	case Behavior::kDeath:
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
	//isDeath_ = true;
	if (behavior_ == Behavior::kDeath) {
		// 敵がやられているなら何もしない
		return;
	}
	// プレイヤーが攻撃中なら敵が死ぬ
	if (player->IsAttack()) {
		// デス演出に切り替え
		if (gameScene_) {

			Vector3 pos = player->GetWorldPosition();
			// 敵と自機の中間位置にエフェクト生成
			Vector3 effectPos;
			effectPos.x = (GetWorldPosition() + pos ).x / 2.0f;
			effectPos.y = (GetWorldPosition() + pos ).y / 2.0f;
			effectPos.z = (GetWorldPosition() + pos ).z / 2.0f;

			gameScene_->CreateEffect(effectPos);
		}
		behaviorRequest_ = Behavior::kDeath;

		// 衝突無効化
		isCollisionDisabled_ = true;
	} 



}


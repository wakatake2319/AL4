#include "Key.h"
#include "Math.h"
#include "Player.h"
#include <numbers>

void Key::Initialize(Model* model, Camera* camera, const Vector3& position) {
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

	rotateTimer_ = 0.0f;
}
void Key::Update() {

	// =========================
	// Behavior遷移の実装
	// =========================
	if (behaviorRequest_ != Behavior::kUnknown) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_;
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {

		// 通常行動
		case Behavior::kGet:
		default:
			counter_ = 0;
			break;
		}
		// 振る舞いリクエストを初期化
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case Behavior::kRoot:


		// タイマー加算
		rotateTimer_ += 1.0f / 60.0f;

		// 回転アニメーション
		worldTransform_.rotation_.y = std::sin(std::numbers::pi_v<float> * 1.0f * rotateTimer_ / kRotateMotionTime); // ワールド行列更新
		WorldTransformUpdate(worldTransform_);
		break;

		// 取る演出
	case Behavior::kGet:
		// 取る演出のカウンターを進める
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);

		// ワールド行列更新
		WorldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isGet_ = true;
		}

		break;
	}
}
void Key::Draw() {
	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}

Vector3 Key::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Key::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Key::OnCollision(const Player* player) {

	// 取るフラグを立てる
	// isGet_ = true;
	if (behavior_ == Behavior::kGet) {
		// 敵がやられているなら何もしない
		return;
	}
	// プレイヤーが攻撃中なら敵が死ぬ
	if (player->IsAttack()) {
		// 取る演出に切り替え
		behaviorRequest_ = Behavior::kGet;

		// 衝突無効化
		isCollisionDisabled_ = true;
	}
}

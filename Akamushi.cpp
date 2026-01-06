#include "Akamushi.h"
#include "Math.h"
#include "Player.h"
#include <numbers>

void Akamushi::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	basePosition_ = position; // 基準位置を保存

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 速度設定
	velocity_ = {-kWalkSpeed, 0, 0};

	rotateTimer_ = 0.0f;
}
void Akamushi::Update() {
	// 終了なら何も表示しない
	if (isFinished_) {
		return;
	}

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

		worldTransform_.rotation_.y += 0.02f; 

		// 回転アニメーション
		WorldTransformUpdate(worldTransform_);
		break;

		// 取る演出
	case Behavior::kGet:
		// 取る演出のカウンターを進める
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.translation_.y += 0.1f;

		// ワールド行列更新
		WorldTransformUpdate(worldTransform_);

		if (counter_ >= kDefeatedTime) {
			isGet_ = true;
			counter_ = kDefeatedTime;
			// 終了扱いにする
			isFinished_ = true;

		}

		break;
	}
}
void Akamushi::Draw() {
	// 終了なら何も表示しない
	if (isFinished_) {
		return;
	}

	// モデル描画
	model_->Draw(worldTransform_, *camera_); 
}

Vector3 Akamushi::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Akamushi::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Akamushi::OnCollision(const Player* player) {
	if (behavior_ == Behavior::kGet)
		return;
		behaviorRequest_ = Behavior::kGet;
		isCollisionDisabled_ = true;
		// プレイヤーに鍵を渡す
		const_cast<Player*>(player)->IsGet();
	
}
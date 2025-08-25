#define NOMINMAX

#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>
#include "MapChipField.h"
void Player::Update() {

	// =========================
	// Behavior遷移の実装
	// =========================
	if (behaviorRequest_ != Behavior::kUnknown) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_;
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {	

		// 通常行動
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		// 攻撃行動
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;

		}
		// 振る舞いリクエストを初期化
		behaviorRequest_ = Behavior::kUnknown; 
	}


	// =====================================
	// 現在のビヘイビアに応じた毎フレームの処理
	// =====================================
	switch (behavior_) {
	// 通常行動
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
	break;

	// 攻撃行動
	case Behavior::kAttack:
		BehaviorAttackUpdate();
	break;
	}



	//BehaviorAttackUpdate();

	//BehaviorRootUpdate();



	WorldTransformUpdate(worldTransform_);
	WorldTransformUpdate(worldTransformAttack_);
}

// ================================
// 通常行動更新
// ================================
void Player::BehaviorRootInitialize() {
}

void Player::BehaviorRootUpdate() {

	
	// 移動入力
	InputMove();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo = {};
	collisionMapInfo.move = velocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.wallContact = false;

	// マップ衝突チェック
	MapCollision(collisionMapInfo);

	// 移動
	worldTransform_.translation_ += collisionMapInfo.move;

	// 天井接触による落下開始
	if (collisionMapInfo.ceilingCollisionFlag) {
		velocity_.y = 0;
	}

	// 接地判定
	isGround(collisionMapInfo);


	// =========================
	// 旋回制御
	// =========================
	if (turnTimer_ > 0.0f) {

		// タイマーを進める
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態に応じた目標角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<int>(lrDirection_)];

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	//WorldTransformUpdate(worldTransform_);

	if (Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_S)) {
		// 攻撃ビヘイビアをリクエスト
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::BehaviorAttackInitialize() {
	// カウンター初期化
	attackParameter_ = 0;

	velocity_ = {};

	// 溜めフェーズから始める
	attackPhase_ = AttackPhase::kAnticipation;
}

// 攻撃時動作
void Player::BehaviorAttackUpdate() {

	// 攻撃時に起こる移動
	const Vector3 attackVelocity = {0.8f, 0.0f, 0.0f};

	// 攻撃動作用の速度
	Vector3 velocity{};

		// 予備動作
	attackParameter_ += 1;

	switch (attackPhase_) {
		// 溜め動作
	case AttackPhase::kAnticipation:
	default: {
		float t = static_cast<float>(attackParameter_) / kAnticipationTime;
		worldTransform_.scale_.z = EaseOut(1.0f, 0.3f, t);
		worldTransform_.scale_.y = EaseOut(1.0f, 1.6f, t);
		// 前進動作へ移行
		if (attackParameter_ >= kAnticipationTime) {
			// 攻撃フェーズを前進動作に変更
			attackPhase_ = AttackPhase::kAction;
			attackParameter_ = 0; // カウンター初期化
		}
		break;
	}
		

		
		// 攻撃動作
	case AttackPhase::kAction: {
		if (lrDirection_ == LRDirection::kRight) {
			velocity = {+attackVelocity.x, +attackVelocity.y, +attackVelocity.z};
		} else {
			velocity = {-attackVelocity.x, -attackVelocity.y, -attackVelocity.z};
		}
		// 攻撃の更新処理
		float t = static_cast<float>(attackParameter_) / kActionTime;
		worldTransform_.scale_.z = EaseOut(0.3f, 1.3f, t);
		worldTransform_.scale_.y = EaseIn(1.6f, 0.7f, t);
		// 前進動作へ移行
		if (attackParameter_ >= kActionTime) {
			// 攻撃フェーズを前進動作に変更
			attackPhase_ = AttackPhase::kRecovery;
			attackParameter_ = 0; // カウンター初期化
		}
	}
		break;

	// 余韻動作
	case AttackPhase::kRecovery: {

		// 攻撃の余韻処理
		float t = static_cast<float>(attackParameter_) / kRecoveryTime;
		worldTransform_.scale_.z = EaseOut(1.3f, 1.0f, t);
		worldTransform_.scale_.y = EaseOut(0.7f, 1.0f, t);
		// 通常行動に移行
		if (attackParameter_ >= kRecoveryTime) {
			behaviorRequest_ = Behavior::kRoot;
		}
		break;
	}
	}

	
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo = {};
	collisionMapInfo.move = velocity;
	collisionMapInfo.landing = false;
	collisionMapInfo.wallContact = false;

	// マップ衝突チェック
	MapCollision(collisionMapInfo);
	// 移動
	//velocity_.x += kAcceleration;
	worldTransform_.translation_ += collisionMapInfo.move;


	//既定の時間経過で攻撃終了して通常状態に戻す
	//if (attackParameter_ >= 60.0f) {
	//    // 通常行動に戻す
	//    behaviorRequest_ = Behavior::kRoot;
	//}

	worldTransformAttack_.translation_ = worldTransform_.translation_;
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;
}

// 初期化
void Player::Initialize(Model* model, Model* modelAttack, Camera* camera, const Vector3& position) {

	assert(model);
	model_ = model;
	modelAttack_ = modelAttack;


	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// ワールド変換攻撃の初期化
	worldTransformAttack_.Initialize();
	worldTransformAttack_.translation_ = worldTransform_.translation_;
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;

	camera_ = camera;
}

// 移動
void Player::InputMove() {

		// 接地状態
	if (onGround_) {

		// =========================
		// 移動入力
		// =========================
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {
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
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {

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
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
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

		// =========================
		// ジャンプ入力　地上時
		// =========================
		if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_W) && onGround_) {
			// ジャンプ初速
			velocity_ += Vector3(0.0f, kJumpAcceleration / 60.0f, 0.0f);

			// 空中ジャンプ使用可
			isAirJump = true;
			airJumpLagTimer = kAirJumpLag;


		}

	} else {
		// =========================
		// 空中にいる時
		// ========================

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttelerationOnsky);
				}

				// 移動
				acceleration.x += kAccelerationOnSky;

				// 左右状態切り替え
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {

				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttelerationOnsky);
				}
				// 移動
				acceleration.x -= kAccelerationOnSky;

				// 左右状態切り替え
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速/減速
			velocity_ += acceleration;

			// 最大速度の制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {

			// 移動入力をしてない場合は減衰させる
			velocity_.x *= (1.0f - kAttelerationOnsky);
		}

		// =========================
		// ジャンプ入力　空中時
		// =========================
		if (Input::GetInstance()->TriggerKey(DIK_UP) || Input::GetInstance()->TriggerKey(DIK_W) && !onGround_) {
			if (isAirJump && airJumpLagTimer <= 0) {
				// ジャンプ初速
				velocity_ += Vector3(0.0f, kAirJumpAcceleration / 60.0f, 0.0f);
				// 空中ジャンプ使用不可
				isAirJump = false;
			}
		}

		// 毎フレーム処理（タイマー減少）
		if (airJumpLagTimer > 0) {
			airJumpLagTimer--;
		}

		// =========================
		// 落下中の制御
		// =========================
		
		// 落下速度
		velocity_ += Vector3(0.0f, -kGravityAcceleration / 60.0f, 0.0f);
		// 落下速度の制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

}

// 衝突判定
void Player::MapCollision(CollisionMapInfo& info) {

	MapCollisionUp(info);
	MapCollisionDown(info);
	MapCollisionRight(info);
	MapCollisionLeft(info);

}

// 上の衝突判定
void Player::MapCollisionUp(CollisionMapInfo& info) {

	// 上昇あり?
	if (info.move.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, knumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の当たり判定の設定
	MapChipField::IndexSet indexSet;
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit)
	{
		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
			// めり込み先のブロック
			MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			info.ceilingCollisionFlag = true;
		}
	}
}

// 下の衝突判定
void Player::MapCollisionDown(CollisionMapInfo& info) {
	// 下降あり?
	if (info.move.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, knumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;


	// 真下の当たり判定を行う
	bool hit = false;

	// 左下点の当たり判定の設定
	MapChipField::IndexSet indexSet;
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		    MapChipField::IndexSet indexSetNow;
		    indexSetNow = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		    if (indexSetNow.yIndex != indexSet.yIndex) {

			    // めり込み先のブロック
			    MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			    info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			    info.landing = true;
		    }
	}
}

// 接地処理
void Player::isGround(const CollisionMapInfo& info) {

	info;

	if (onGround_) {
		// 地面に接地しているときの処理
		if (velocity_.y > 0.0f) {
		onGround_ = false;
		} else {
			// =========================
			// 落下判定
			// =========================
			// 02_08スライド19枚目(このelseブロック全部)
			std::array<Vector3, knumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定
			bool hit = false;
			// 左下点の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の判定
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下なら空中状態に切り替え
			if (!hit) {
				onGround_ = false;
			}
		}

	} else {
		// 地面から離れているとき
		if (info.landing) {
		// 着地状態に切り替える
		onGround_ = true;
		// 着地時にX速度を減衰
		velocity_.x *= (1.0f - kAttenuationLanding);
		// Y速度を0にする
		velocity_.y = 0.0f;
		}
	}
}

// 壁に当たったか
void Player::isWall(const CollisionMapInfo& info) {

	if (info.wallContact) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

// 右の衝突判定
void Player::MapCollisionRight(CollisionMapInfo& info) { 
	// 右移動あり?
	if (info.move.x <= 0) {
		return;
	}

		// 移動後の4つの角の座標
	std::array<Vector3, knumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 右の当たり判定を行う
	bool hit = false;

	MapChipField::IndexSet indexSet;

	// 右上点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));
			// めり込み先のブロック
			MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			info.wallContact = true;
		}
	}
}

// 左の衝突判定
void Player::MapCollisionLeft(CollisionMapInfo& info) { 	
	// 左移動あり?
	if (info.move.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, knumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	// 右の当たり判定を行う
	bool hit = false;

	MapChipField::IndexSet indexSet;

	// 左上点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));
		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
			// めり込み先のブロック
			MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::max(0.0f, rect.right - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			info.wallContact = true;
		}
	}
}


// 指定した角の座標計算
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];


}

// 描画
void Player::Draw() {

	// モデル描画
	model_->Draw(worldTransform_, *camera_);

	// 攻撃エフェクト描画
	if (behavior_ == Behavior::kAttack) {

		switch (attackPhase_) {
			// 溜め動作
		case AttackPhase::kAnticipation:
		default: 
			// 攻撃フェーズが溜め動作の時は、攻撃エフェクトを描画しない
			break;
		

		// 攻撃動作
		case AttackPhase::kAction: 
		// 余韻動作
		case AttackPhase::kRecovery: 
			modelAttack_->Draw(worldTransformAttack_, *camera_);
			break;
		}	
	}
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 敵との衝突処理
void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy;

	if (IsAttack()) {
		return;
	}
	// デスフラグを立てる
	isDeath_ = true;
}

#define NOMINMAX

#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>
#include "MapChipField.h"

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

void Player::InputMove() {

		// 接地状態
	if (onGround_) {

		// =========================
		// 移動入力
		// =========================
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
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
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
		// ジャンプ入力
		// =========================
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0.0f, kJumpAcceleration / 60.0f, 0.0f);
		}

	} else {
		// =========================
		// 空中にいる時
		// ========================

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

	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の当たり判定の設定
	MapChipField::IndexSet indexSet;
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
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

	// 真下の当たり判定を行う
	bool hit = false;

	// 左下点の当たり判定の設定
	MapChipField::IndexSet indexSet;
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
			// めり込みを排除する方向に移動量を設定する
			indexSet = mapchipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
			// めり込み先のブロック
			MapChipField::Rect rect = mapchipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			info.landing = true;
		
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

	// 右の当たり判定を行う
	bool hit = false;

	MapChipField::IndexSet indexSet;

	// 右上点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
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
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
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

	// 右の当たり判定を行う
	bool hit = false;

	MapChipField::IndexSet indexSet;

	// 右上点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapchipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapchipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
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
			info.move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
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




void Player::Update() {

	// 移動入力
	InputMove();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo = {};
	collisionMapInfo.move = velocity_;

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
	// 着地処理
	// =========================	
	/*
	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中?
	if (velocity_.y < 0.0f) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}	


	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み要素
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAtteleration);
			// 落下速度をリセット
			velocity_.y = 0.0f;
			// 着地
			onGround_ = true;
		}
	}
	*/

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

	WorldTransformUpdate(worldTransform_);
}

void Player::Draw() {

	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}
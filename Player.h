#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

// 前方宣言
class MapChipField;
class Enemy;



class Player {
public:

	// 左右の振り向き
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 角
	enum Corner {
		// 右下
		kRightBottom,
		// 左下
		kLeftBottom,
		// 右上
		kRightTop,
		// 左上
		kLeftTop,

		// 要素数
		knumCorner
	};

	// ビヘイビア
	enum class Behavior {
		// 未定義
		kUnknown = -1,
		// 通常状態
		kRoot,
		// 攻撃中
		kAttack,
	};

	// 攻撃フェーズ
	enum class AttackPhase {
		// 未定義
		kUnknown = -1,
		// 溜め
		kAnticipation,
		// 攻撃
		kAction,
		// 余韻
		kRecovery,
	};

	// 初期化
	void Initialize(Model* model, Model* modelAttack, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// 速度加算
	const Vector3& GetVelocity() const { return velocity_; }

	// マップチップフィールド
	void SetMapChipField(MapChipField* mapchipField) { mapchipField_ = mapchipField; }

		// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABB取得関数
	AABB GetAABB();

	void OnCollision(const Enemy* enemy);

	// デスフラグのgetter
	bool IsDeath() const { return isDeath_; }

	// 通常行動更新
	void BehaviorRootUpdate();

	// 攻撃行動更新
	void BehaviorAttackUpdate();

	// 通常行動初期化
	void BehaviorRootInitialize();

	// 攻撃行動初期化
	void BehaviorAttackInitialize();

	// 攻撃しているかどうか
	bool IsAttack() const { return behavior_ == Behavior::kAttack && attackPhase_ == AttackPhase::kAction; }


private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_ = {};

	// ================================
	// 移動
	// ================================
	// 地上慣性移動
	static inline const float kAcceleration = 0.01f;
	// 地上減速
	static inline const float kAtteleration = 0.05f; 
	// 空中慣性移動
	static inline const float kAccelerationOnSky = 0.008f;
	// 空中減速
	static inline const float kAttelerationOnsky = 0.06f;
	// 最大速度
	static inline const float kLimitRunSpeed = 0.3f;


	// ===============================
	// 振り向き
	// ===============================
	// 角度補強
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 0.3f;
	// 左右の振り向き
	LRDirection lrDirection_ = LRDirection::kRight;


	// ===============================
	// 着地
	// ===============================
	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;

	// ================================
	// プレイヤーの当たり判定
	// ================================
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;


	static inline const float kBlank = 0.04f;

	// ======================
	// 移動処理
	// ======================
	void InputMove();
	// マップチップとの当たり判定情報
	struct CollisionMapInfo {

		// 天井衝突フラグ
		bool ceilingCollisionFlag = false;

		// 着地フラグ
		bool landing = false;

		// 壁接触フラグ
		bool wallContact = false;

		// 移動量
		Vector3 move;
	};

	// 移動量を加味して衝突判定する
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionUp(CollisionMapInfo& info);
	void MapCollisionDown(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);

	// 指定した過度の座標計算
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 当たり判定
	MapChipField* mapchipField_ = nullptr;

	// 接地状態の切り替え
	void isGround(const CollisionMapInfo& info);

	// 壁接触状態の切り替え
	void isWall(const CollisionMapInfo& info);

	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.1f;
	// 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;
	// 壁接触時の速度減衰率
	static inline const float kAttenuationWall = 0.2f;

	// デスフラグ
	bool isDeath_ = false;

	// ================================
	// Behaviorの定義
	// ================================
	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いのリクエスト
	Behavior behaviorRequest_ = Behavior::kUnknown;


	// ================================
	// 行動カウンター
	// ================================
	// 攻撃ギミックの経過時間カウンター
	uint32_t attackParameter_ = 0;

	// 攻撃フェーズ
	AttackPhase attackPhase_ = AttackPhase::kUnknown;
	// 予備動作の時間
	static inline const uint32_t kAnticipationTime = 8;
	// 前進動作の時間
	static inline const uint32_t kActionTime = 5;
	// 余韻動作の時間
	static inline const uint32_t kRecoveryTime = 12;



	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// カメラ
	Camera* camera_;

	// 攻撃エフェクト
	Model* modelAttack_ = nullptr;
	WorldTransform worldTransformAttack_;

};
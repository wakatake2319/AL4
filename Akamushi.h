#pragma once
#include "KamataEngine.h"
#include "Math.h"
using namespace KamataEngine;

class Player;

class Akamushi {

public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABB取得関数
	AABB GetAABB();

	// 衝突判定
	void OnCollision(const Player* player);

	// 取ったかどうか
	bool isGet() const { return isGet_; }

	// 衝突無効化
	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

	// ビヘイビア
	enum class Behavior {
		// 未定義
		kUnknown = -1,
		// 通常状態
		kRoot,
		// 取る演出
		kGet,
	};

		bool IsFinished() { return isFinished_; }


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.02f;
	// 速度
	Vector3 velocity_ = {};

	// ===================
	// アニメーション
	// ===================
	// 最初の角度
	static inline const float kRotateMotionAngleStart = 0.0f;
	// 最後の角度
	static inline const float kRotateMotionAngleEnd = 30.0f;
	// アニメーションの周期となる時間
	static inline const float kRotateMotionTime = 1.0f;
	// 経過時間
	float rotateTimer_ = 0.0f;

	// 当たり判定の大きさ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 取るフラグ
	bool isGet_ = false;

	Behavior behavior_ = Behavior::kRoot;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f; // カウンター

	// 衝突判定無効化
	bool isCollisionDisabled_ = false;

	// 基準位置
	Vector3 basePosition_;

		bool isFinished_ = false;
};
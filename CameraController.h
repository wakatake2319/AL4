#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

// 前方宣言
class Player;

class CameraController {
public:
	struct Rect {
		// 左端
		float left = 0.0f;
		// 右端
		float right = 1.0f;
		// 下端
		float bottom = 0.0f;
		// 上端
		float top = 1.0f;
	};

	// 初期化
	void Initialize(Camera *camera);
	// 更新
	void Update();

	// 追従対象
	void SetTarget(Player* target) { target_ = target; }

	// カメラのリセット
	void Reset();

	// カメラ移動範囲
	void SetMovableArea(Rect area) { movableArea_ = area; }

private:

	// カメラ
	Camera* camera_ = nullptr;

	// 追従対象
	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差
	Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	// カメラの目標座標
	Vector3 objectivePoint_;

	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f;

	// 速度掛け率
	static inline const float kVelocityBias = 18.0f;

	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
};
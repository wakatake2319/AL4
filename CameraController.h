#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

// 前方宣言
class Player;

class CameraController {
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();

	// 追従対象
	void SetTarget(Player* target) { target_ = target; }

	// カメラのリセット
	void Reset();

private:

	// カメラ
	Camera* camera_ = nullptr;

	// 追従対象
	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差
	Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f};

};
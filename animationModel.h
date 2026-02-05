#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;


class animationModel {

public:
	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

private:

	// モデル
	Model* model_;

	// ワールド変換
	WorldTransform worldTransform_;

	// カメラ
	Camera* camera_;
};

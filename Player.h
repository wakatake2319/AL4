#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Player {
public:

	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

		// 更新
	void Update();

	// 描画
	void Draw();

	   // ワールド変換更新処理
	void WorldTransformUpdate();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_ = {0.0f,0.1f,0.0f};

	// 慣性移動
	static inline const float kAcceleration = 0.1f; 

	// モデル
	Model* model_ = nullptr;
	
	// カメラ
	Camera* camera_;
};
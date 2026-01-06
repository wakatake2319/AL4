#pragma once
#include "KamataEngine.h"
#include "Math.h"

class wakame {
public:

	// 初期化
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_;

	// 
	wakame* sun_ = nullptr;

	// 基準位置
	Vector3 basePosition_;
};
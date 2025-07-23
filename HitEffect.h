#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;


class HitEffect {
public:
	// 更新
	void Update();
	// 描画
	void Draw();

	// モデルの設定
	static void SetModel(Model* model) { model_ = model; }
	// カメラの設定
	static void SetCamera(Camera* camera) { camera_ = camera; }

		// インスタンス生成と初期化
	static HitEffect* Create(const Vector3& position);

private:

	// 初期化
	void Initialize(const Vector3& position);


	// モデル（借りてくる用）
	static Model* model_;

	// カメラ（借りてくる用）
	static Camera* camera_;

	// 円のワールドトランスフォーム
	WorldTransform circleWorldTransform_;

	ObjectColor objectColor_;
};
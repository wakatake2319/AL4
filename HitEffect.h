#pragma once
#include <cstdint>
#include "KamataEngine.h"

using namespace KamataEngine;


class HitEffect {
public:

	enum class State {
		kSpread, // 拡大中
		kFade,   // フェードアウト中
		kDead    // 死亡
	};




	// モデルの設定
	static void SetModel(Model* model) { model_ = model; }
	// カメラの設定
	static void SetCamera(Camera* camera) { camera_ = camera; }

	// インスタンス生成と初期化
	static HitEffect* Create(const Vector3& position);

	// 更新
	void Update();
	// 描画
	void Draw();

	// 死亡状態の取得
	bool IsDead() const { return state_ == State::kDead; }


private:
	HitEffect() = default;


	// 初期化
	void Initialize(const Vector3& position);


	// モデル（借りてくる用）
	static Model* model_;

	// カメラ（借りてくる用）
	static Camera* camera_;

	// 楕円エフェクトの数
	static const inline uint32_t kellipseEffectNum = 2;

	// 楕円のワールドトランスフォーム
	std::array<WorldTransform, kellipseEffectNum> ellipseWorldTransforms_;

	// 円のワールドトランスフォーム
	WorldTransform circleWorldTransform_;

	State state_ = State::kSpread;

	ObjectColor objectColor_;
};
#pragma once
#include "Fade.h"
#include "KamataEngine.h"

using namespace KamataEngine;

class ClearScene {
public:
	// シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン
		kFadeOut, // フェードアウト
	};

	~ClearScene();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeClearMove = 2.0f;

	// ビュープロジェクション
	Camera camera_;
	WorldTransform worldTransformClear_;
	WorldTransform worldTransformPlayer_;

	// クリアシーンのテクスチャ
	uint32_t clearSceneTH_ = 0;
	Sprite* clearSceneSprite_ = nullptr;

	float counter_ = 0.0f;

	// 終了フラグ
	bool finished_ = false;

	// フェード
	Fade* fade_ = nullptr;

	// 現在のフェード
	Phase phase_ = Phase::kFadeIn;
};

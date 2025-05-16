#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include <vector>

using namespace KamataEngine;


class GameScene {
public:
	// 初期化
	void Initialize();

	GameScene();
	~GameScene();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// モデル
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// カメラ
	Camera camera_;

	// スカイドーム
	Skydome* skydome_ = nullptr;


	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

};

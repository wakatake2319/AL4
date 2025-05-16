#pragma once
#include "KamataEngine.h"
#include <vector>

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
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;


	KamataEngine::Model* modelBlock_ = nullptr;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;


	bool isDebugCameraActive_ = false;
};

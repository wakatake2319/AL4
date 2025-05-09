#pragma once
#include "KamataEngine.h"

class GameScene {
public:
	void Initialize();

	GameScene();

	~GameScene();

	void Update();

	void Draw();

private:
	uint32_t textureHndle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	uint32_t soundDateHndle_ = 0;
	uint32_t voiceHndle_ = 0;

	float inputFloat3[3] = {0, 0, 0};
};
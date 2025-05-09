#pragma once
#include "KamataEngine.h" 
#include "Player.h"


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
	uint32_t textureHndle_ = 0;
	Model* model_ = nullptr;
	Camera camera_; 
	// 自キャラ
	Player* player_ = nullptr;
};

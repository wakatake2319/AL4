#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include <vector>
#include "Player.h"

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

	// カメラ
	Camera camera_;

	// スカイドーム
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;



	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	Sprite* sprite_ = nullptr;
	// 3Dモデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// プレイヤー
	Player* player_ = nullptr;
	// プレイヤーモデル
	Model* player_model_ = nullptr;
};

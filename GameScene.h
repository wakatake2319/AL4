#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "Player.h"

using namespace KamataEngine;


class GameScene {
public:

	~GameScene();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 表示ブロックの生成
	void GenerateBlocks();


private:




	// ブロック
	Model* modelBlock_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;


	// カメラ
	Camera camera_;

	// スカイドーム
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;


	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;


	// マップチップフィールド
	MapChipField* mapChipField_;

	// プレイヤー
	Player* player_ = nullptr;
	Model* player_model_ = nullptr;

};

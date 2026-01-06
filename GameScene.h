#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "Player.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeatParticles.h"
#include "Fade.h"
#include "Akamushi.h"
#include "wakame.h"
#include <filesystem>

using namespace KamataEngine;


class GameScene {
public:

	enum class Result {
		kNone,
		kClear,
		kDead,
	};

	~GameScene();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 表示ブロックの生成
	void GenerateBlocks();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	bool IsFinished() const { return finished_; }

	Result GetResult() const { return result_; }

private:
	// ゲームのフェーズ
	enum class Phase 
	{
		kFadeIn, // フェードイン
		kplay,// ゲームプレイ
		kDeath,// デス演出
		kClear,   // クリア演出
		kFadeOut, // フェードアウト
	};

	// ゲームの現在のフェーズ
	Phase phase_;

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
	Model* modelAttack_ = nullptr;

	// カメラコントローラー
	CameraController* cameraController_ = nullptr;

	// エネミー
	Enemy* enemy_ = nullptr;
	Model* enemy_model_ = nullptr;
	// 敵の複数化
	std::list<Enemy*> enemies_;

	// 鍵
	Akamushi* akamushi_ = nullptr;
	Model* akamushi_model_ = nullptr;


	// デスパーティクル
	DeathParticles* deathParticles_ = nullptr;

	// デスパーティクルのモデル
	Model* deathParticle_model_ = nullptr;

	// フェーズの切り替え
	void ChangePhase();

	// 終了フラグ
	bool finished_ = false;

	Fade* fade_ = nullptr;

	Result result_ = Result::kNone;

	// わかめ
	wakame* wakame_ = nullptr;
	Model* modelwakame_ = nullptr;

	// わかめの複数化
	std::list<wakame*> Wakames_;


};

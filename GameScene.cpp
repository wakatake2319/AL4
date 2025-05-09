#include "GameScene.h"

using namespace KamataEngine;
// 初期化
void GameScene::Initialize() {
	textureHndle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();

	camera_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHndle_, &camera_);
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
}

// 更新
void GameScene::Update() { player_->Update(); }

// 描画
void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	player_->Draw();
	Model::PostDraw();
}
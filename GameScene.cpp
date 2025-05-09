#include "GameScene.h"
#include "ImGui.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	textureHndle_ = TextureManager::Load("pijotto.png");
	sprite_ = Sprite::Create(textureHndle_, {100, 50});

	model_ = Model::Create();

	worldTransform_.Initialize();
	camera_.Initialize();

	soundDateHndle_ = Audio::GetInstance()->LoadWave("mokugyo.wav");
	Audio::GetInstance()->PlayWave(soundDateHndle_);
	voiceHndle_ = Audio::GetInstance()->PlayWave(soundDateHndle_, true);

	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	// デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(voiceHndle_);
	}

#ifdef _DEBUG
	ImGui::Begin("Debug1");
	// デバックテキストの表示
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

	ImGui::ShowDemoWindow();
#endif

	// デバッグカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());

	sprite_->Draw();

	Sprite::PostDraw();

	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHndle_);

	Model::PostDraw();

	// ライン描画
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
}
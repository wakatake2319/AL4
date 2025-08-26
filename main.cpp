#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// タイトルシーンのインスタンス生成
TitleScene* titleScene = nullptr;
// ゲームシーン
GameScene* gameScene = nullptr;
// ゲームクリアのインスタンス生成
ClearScene* clearScene = nullptr;


// シーン
enum class Scene {
	kUnknown = 0,
	kTitle, // タイトル
	kGame,  // ゲーム
	kClear, // クリア
};

// 現在のシーン
Scene scene = Scene::kUnknown;

// ==========================
// シーン切り替え処理
// ==========================
void ChangScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;

			// 旧シーンの開放
			delete titleScene;    
			titleScene = nullptr; 

			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {

				// 死亡 → タイトルへ戻る
				scene = Scene::kTitle;
				delete gameScene;
				gameScene = nullptr;
				titleScene = new TitleScene();
				titleScene->Initialize();
			



		}
		break;	
	case Scene::kClear:
		if (clearScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの開放
			delete clearScene;
			clearScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
	}
}

// ==========================
// シーンの更新
// ==========================
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	}
}

// ==========================
// シーンの描画
// ==========================
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_18_タケウチ_ハルカ_AL3");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 最初のシーンの初期化
	scene = Scene::kTitle; 
	titleScene = new TitleScene;
	titleScene->Initialize();

	// ゲームシーンのインスタンス生成
	//GameScene* gameScene = new GameScene();
	// ゲームシーンの初期化
	//gameScene->Initialize();




	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// シーンの切り替え
		ChangScene();
		// 現在シーンの更新
		UpdateScene();



		// 描画開始
		dxCommon->PreDraw();

		// 描画処理をここに記述
	
		// 現在シーンの描画
		DrawScene();

		
		// ゲームシーンの描画
		//gameScene->Draw();

		dxCommon->PostDraw();
	}

	// タイトルシーンの開放
	delete titleScene;
	// ゲームシーンの開放
	delete gameScene;
	// クリアシーンの開放
	delete clearScene;
	// nullptrの代入
	//gameScene = nullptr;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}

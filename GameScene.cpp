#include "GameScene.h"

using namespace KamataEngine;

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[row][col] += m1.m[row][k] * m2.m[k][col];
			}
		}
	}
	return result;
}

GameScene::~GameScene() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete deathParticles_;
	delete deathParticle_model_;
	delete key_;
	delete modelSun_;
	delete sun_;
	delete fade_;
	delete cameraController_;
	delete skydome_;
	delete player_;

}

// 初期化
void GameScene::Initialize() {

	// マップチップの初期化
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// プレイヤーの初期化
	player_ = new Player();
	// プレイヤーのモデル
	player_model_ = Model::CreateFromOBJ("player");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 21);
	modelAttack_ = Model::CreateFromOBJ("attack_effect");

	player_->SetMapChipField(mapChipField_);
	player_->Initialize(player_model_, modelAttack_, &camera_, playerPosition);

	modelBlock_ = Model::CreateFromOBJ("block");

	// 3Dモデル(天球)の生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	camera_.Initialize();

	// カメラコントロールの初期化
	cameraController_ = new CameraController;
	cameraController_->Initialize(&camera_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	// カメラコントローラーの移動範囲の指定
	CameraController::Rect cameraArea = { 
		12.0f, 
		MapChipField::GetNumBlockHorizontal() * MapChipField::kBlockWidth - 12.0f,
		8.0f, // 下の範囲
	    MapChipField::GetNumBlockVirtical() * MapChipField::kBlockHeight - 10.0f}; // 上の範囲
	cameraController_->SetMovableArea(cameraArea);

	// 敵モデル
	enemy_model_ = Model::CreateFromOBJ("enemy");

	for (int32_t i = 0; i < 2; ++i) {
		// エネミーの初期化
		Enemy* newenemy_ = new Enemy;
		// エネミーの初期位置
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12 + i * 4, 21);
		newenemy_->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newenemy_);
	}


	for (int32_t i = 0; i < 2; ++i) {
		// エネミーの初期化
		Enemy* newenemy2_ = new Enemy;
		// エネミーの初期位置
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(37 + i * 4, 21);
		newenemy2_->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newenemy2_);
	}

	for (int32_t i = 0; i < 2; ++i) {
		// エネミーの初期化
		Enemy* newenemy3_ = new Enemy;
		// エネミーの初期位置
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(20 + i * 4, 10 - i);
		newenemy3_->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newenemy3_);
	}
	for (int32_t i = 0; i < 3; ++i) {
		// エネミーの初期化
		Enemy* newenemy3_ = new Enemy;
		// エネミーの初期位置
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(36 + i * 4, 10 + i);
		newenemy3_->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newenemy3_);
	}
	for (int32_t i = 0; i < 3; ++i) {
		// エネミーの初期化
		Enemy* newenemy3_ = new Enemy;
		// エネミーの初期位置
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(60, 10 + i);
		newenemy3_->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newenemy3_);
	}
	for (int32_t i = 0; i < 3; ++i) {
		// エネミーの初期化
		Enemy* newenemy3_ = new Enemy;
		// エネミーの初期位置
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(70 + i * 4, 21 - i);
		newenemy3_->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newenemy3_);
	}

	for (int32_t i = 0; i < 3; ++i) {
		// エネミーの初期化
		Enemy* newenemy3_ = new Enemy;
		// エネミーの初期位置
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(87 + i * 2, 13);
		newenemy3_->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newenemy3_);
	}

	// モデル読み込み
	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");

	key_ = new Key();
	// 鍵のモデル
	key_model_ = Model::CreateFromOBJ("key");
	Vector3 keyPosition = mapChipField_->GetMapChipPositionByIndex(108, 21);
	key_->Initialize(key_model_, &camera_, keyPosition);


	// ゲームプレイフェーズから開始
	phase_ = Phase::kFadeIn;

	// フェードの初期化
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
	
	// 3Dモデル(天球)の生成
	modelSun_ = Model::CreateFromOBJ("sun", true);
	sun_ = new Sun();
	Vector3 sunPosition = {-10, 20,20};
	sun_->Initialize(modelSun_, &camera_, sunPosition);
}


void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kplay:
		if (player_->IsDeath()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			// デスパーティクルの初期化
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticle_model_, &camera_, deathParticlesPosition);
		} else if (key_->isGet()) {
			// クリア演出フェーズに切り替え
			phase_ = Phase::kClear;

		}
		break;
	case Phase::kDeath:
		break;
	case Phase::kClear:

		break;
	}
}

// 表示ブロックの生成
void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の総素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}



// 更新
void GameScene::Update() {

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->isDeath()) {
			delete enemy;
			return true; // 削除対象
		}
		return false; // 削除対象ではない
	});


	ChangePhase();

	switch (phase_) {


	// フェードインフェーズの処理
	case Phase::kFadeIn:
		// フェードの更新
		fade_->Update();
		if (fade_->IsFinished()) {

			fade_->Start(Fade::Status::FadeOut, 1.0f);

			// フェードインが終了したらゲームプレイフェーズへ移行
			phase_ = Phase::kplay;
		}

		// 天球の更新
		skydome_->Update();

		// プレイヤーの更新
		player_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 鍵の更新
		key_->Update();

		// 太陽の更新
		sun_->Update();



		// カメラコントローラーの更新
		cameraController_->Update();

#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif
		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			camera_.TransferMatrix();
		} else {
			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				// アフィン変換~DirectXに転送
				WorldTransformUpdate(*worldTransformBlock);
			}
		}
		break;


	// ゲームプレイフェーズの処理
	case Phase::kplay:

		// 天球の更新
		skydome_->Update();

		// プレイヤーの更新
		player_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 鍵の更新
		key_->Update();


		// 太陽の更新
		sun_->Update();


	

		// カメラコントローラーの更新
		cameraController_->Update();
		#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
		#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			// デバッグカメラのビュー行列
			camera_.matView = debugCamera_->GetCamera().matView;
			// デバッグカメラのプロジェクション行列
			camera_.matProjection = debugCamera_->GetCamera().matProjection;

			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				// アフィン変換行列の作成
				Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				worldTransformBlock->matWorld_ = worldMatrix;
				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		// 全ての当たり判定
		CheckAllCollisions();

		break;

	case Phase::kDeath:
		// デスパーティクルのフェードアウト
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
		}

		// 天球の更新
		skydome_->Update();

		// カメラコントローラーの更新
		cameraController_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 鍵の更新
		key_->Update();

		// 太陽の更新
		sun_->Update();



		// デスパーティクルの更新
		if (deathParticles_) {
			deathParticles_->Update();
		}



		break;

	case Phase::kClear:
		// クリア演出の更新
		// クリア演出が終了したらフェードアウトフェーズへ移行
		if (key_ && key_->IsFinished()) {
			phase_ = Phase::kFadeOut;
		}
		// 天球の更新
		skydome_->Update();

		// カメラコントローラーの更新
		cameraController_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 太陽の更新
		sun_->Update();



		break;


	case Phase::kFadeOut:
		// フェードの更新
		fade_->Update();

		// フェードアウトが終了したらゲームシーンを終了
		if (fade_->IsFinished()) {

			// 死亡チェック
			if (player_->IsDeath()) {
				phase_ = Phase::kDeath;
				finished_ = true;
				result_ = Result::kDead;
				return;
			}

			// ゴールチェック
			if (player_->IsGet()) {
				phase_ = Phase::kClear;
				finished_ = true;
				result_ = Result::kClear;
				return;
			}
		}

		// 天球の更新
		skydome_->Update();

		// カメラコントローラーの更新
		cameraController_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 鍵の更新
		key_->Update();

		// 太陽の更新
		sun_->Update();



		break;
	}

	//debugCamera_->Update();
}

// 描画
void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 天球描画
	skydome_->Draw();

	// 太陽描画
	sun_->Draw();

	// プレイヤー描画
	if (!player_->IsDeath()) {
		player_->Draw();
	}

	// エネミー描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 鍵の描画
	if (!key_->isGet()) {
		key_->Draw();
	}



	// ブロック描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// デスパーティクル描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	Model::PostDraw();

	fade_->Draw();
}

void GameScene::CheckAllCollisions() {

	AABB aabb1, aabb2;

#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {

			if (enemy->IsCollisionDisabled())
				// 衝突判定を行わない
				continue;

			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollisionEnemy(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}

		// ==============================
		// 自キャラと鍵の当たり判定
		// ==============================
		{
			aabb1 = player_->GetAABB();
			aabb2 = key_->GetAABB();

			if (IsCollision(aabb1, aabb2)) {

				player_->OnCollisionKey(key_);
				key_->OnCollision(player_);
			}
		}

	}
#pragma endregion
#pragma region 自キャラの攻撃と敵キャラの当たり判定
	{
		// 攻撃中でなければ何もしない
		if (player_->IsAttackHitActive()) {

			// 攻撃判定のAABB
			AABB attackAABB = player_->GetAttackAABB();

			for (Enemy* enemy : enemies_) {

				if (enemy->IsCollisionDisabled())
					continue;

        if (IsCollision(attackAABB, enemy->GetAABB())) {
					enemy->OnHitByAttack();
				}
			}
		}
	}
#pragma endregion
}
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

// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {

	Matrix4x4 result{};

	Matrix4x4 scaleMatrix{};
	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[0][1] = 0;
	scaleMatrix.m[0][2] = 0;
	scaleMatrix.m[0][3] = 0;
	scaleMatrix.m[1][0] = 0;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[1][2] = 0;
	scaleMatrix.m[1][3] = 0;
	scaleMatrix.m[2][0] = 0;
	scaleMatrix.m[2][1] = 0;
	scaleMatrix.m[2][2] = scale.z;
	scaleMatrix.m[2][3] = 0;
	scaleMatrix.m[3][0] = 0;
	scaleMatrix.m[3][1] = 0;
	scaleMatrix.m[3][2] = 0;
	scaleMatrix.m[3][3] = 1;

	// X軸回転行列
	Matrix4x4 rotateMatrixX = {};
	rotateMatrixX.m[0][0] = 1.0f;
	rotateMatrixX.m[0][1] = 0.0f;
	rotateMatrixX.m[0][2] = 0.0f;
	rotateMatrixX.m[0][3] = 0.0f;
	rotateMatrixX.m[1][0] = 0.0f;
	rotateMatrixX.m[1][1] = std::cos(rot.x);
	rotateMatrixX.m[1][2] = std::sin(rot.x);
	rotateMatrixX.m[1][3] = 0.0f;
	rotateMatrixX.m[2][0] = 0.0f;
	rotateMatrixX.m[2][1] = -std::sin(rot.x);
	rotateMatrixX.m[2][2] = std::cos(rot.x);
	rotateMatrixX.m[2][3] = 0.0f;
	rotateMatrixX.m[3][0] = 0.0f;
	rotateMatrixX.m[3][1] = 0.0f;
	rotateMatrixX.m[3][2] = 0.0f;
	rotateMatrixX.m[3][3] = 1.0f;

	// Y軸回転行列
	Matrix4x4 rotateMatrixY = {};
	rotateMatrixY.m[0][0] = std::cos(rot.y);
	rotateMatrixY.m[0][1] = 0.0f;
	rotateMatrixY.m[0][2] = -std::sin(rot.y);
	rotateMatrixY.m[0][3] = 0.0f;
	rotateMatrixY.m[1][0] = 0.0f;
	rotateMatrixY.m[1][1] = 1.0f;
	rotateMatrixY.m[1][2] = 0.0f;
	rotateMatrixY.m[1][3] = 0.0f;
	rotateMatrixY.m[2][0] = std::sin(rot.y);
	rotateMatrixY.m[2][1] = 0.0f;
	rotateMatrixY.m[2][2] = std::cos(rot.y);
	rotateMatrixY.m[2][3] = 0.0f;
	rotateMatrixY.m[3][0] = 0.0f;
	rotateMatrixY.m[3][1] = 0.0f;
	rotateMatrixY.m[3][2] = 0.0f;
	rotateMatrixY.m[3][3] = 1.0f;

	// Z軸回転行列
	Matrix4x4 rotateMatrixZ = {};
	rotateMatrixZ.m[0][0] = std::cos(rot.z);
	rotateMatrixZ.m[0][1] = std::sin(rot.z);
	rotateMatrixZ.m[0][2] = 0.0f;
	rotateMatrixZ.m[0][3] = 0.0f;
	rotateMatrixZ.m[1][0] = -std::sin(rot.z);
	rotateMatrixZ.m[1][1] = std::cos(rot.z);
	rotateMatrixZ.m[1][2] = 0.0f;
	rotateMatrixZ.m[1][3] = 0.0f;
	rotateMatrixZ.m[2][0] = 0.0f;
	rotateMatrixZ.m[2][1] = 0.0f;
	rotateMatrixZ.m[2][2] = 1.0f;
	rotateMatrixZ.m[2][3] = 0.0f;
	rotateMatrixZ.m[3][0] = 0.0f;
	rotateMatrixZ.m[3][1] = 0.0f;
	rotateMatrixZ.m[3][2] = 0.0f;
	rotateMatrixZ.m[3][3] = 1.0f;

	// X、Y、Z軸回転行列の合成（Z→Y→X）
	Matrix4x4 rotateMatrixXYZ = {};
	rotateMatrixXYZ = Multiply(rotateMatrixX, Multiply(rotateMatrixY, rotateMatrixZ));

	Matrix4x4 translateMatrix = {};
	translateMatrix.m[0][0] = 1;
	translateMatrix.m[0][1] = 0;
	translateMatrix.m[0][2] = 0;
	translateMatrix.m[0][3] = 0;
	translateMatrix.m[1][0] = 0;
	translateMatrix.m[1][1] = 1;
	translateMatrix.m[1][2] = 0;
	translateMatrix.m[1][3] = 0;
	translateMatrix.m[2][0] = 0;
	translateMatrix.m[2][1] = 0;
	translateMatrix.m[2][2] = 1;
	translateMatrix.m[2][3] = 0;
	translateMatrix.m[3][0] = translate.x;
	translateMatrix.m[3][1] = translate.y;
	translateMatrix.m[3][2] = translate.z;
	translateMatrix.m[3][3] = 1;

	result = Multiply(scaleMatrix, Multiply(rotateMatrixXYZ, translateMatrix));

	return result;
}


GameScene::GameScene() {}

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
}

// 初期化
void GameScene::Initialize() {

	modelBlock_ = Model::CreateFromOBJ("block");


	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	camera_.Initialize();


	// マップチップの初期化
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();


	// プレイヤーの初期化
	player_ = new Player();

	// プレイヤーのモデル
	//player_model_ = Model::CreateFromOBJ(モデルネーム,"player");

	// プレイヤーの初期配置をマップチップ単位で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(0, 0);
	player_->Initialize(modelBlock_, &camera_, playerPosition);

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
	skydome_->Update();
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
	debugCamera_->Update();
}

// 描画
void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}
	skydome_->Draw();
	Model::PostDraw();
}



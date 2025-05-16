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

// 初期化
void GameScene::Initialize() {

	modelBlock_ = Model::Create();
	camera_.Initialize();



	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);


	// ============================================
	// ↓ジェネレイトブロックにいれるコード
	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1子分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	// 要素数を変更する
	// 列数設定(縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		// 1列の総素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if ((i + j) % 2 == 1)
				continue;

			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
	// ============================================
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
}
// 更新
void GameScene::Update() {
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
	// デバッグカメラの更新
	debugCamera_->Update();

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
	Model::PostDraw();
}

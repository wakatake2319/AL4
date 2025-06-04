#include "CameraController.h"
#include "Player.h"

	// 初期化
void CameraController::Initialize() { 
	// プレイヤーの初期化
	camera_ = new Camera();


}
// 更新
void CameraController::Update() {


}

// リセット
void CameraController::Reset() { 
	// 追従対象のワールドトランスフォーム
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;

}
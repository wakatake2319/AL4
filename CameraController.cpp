#include "CameraController.h"
#include "Player.h"

	// 初期化
void CameraController::Initialize(Camera* camera) { 
	// プレイヤーの初期化
	camera_ = camera;


}
// 更新
void CameraController::Update() {

	// 追従対象のワールドトランスフォーム
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	const Vector3& targetVelocity = target_->GetVelocity();
	objectivePoint_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;
	// 座標補間によりゆったり追従
	camera_->translation_ = Lerp(camera_->translation_, objectivePoint_, kInterpolationRate);

	// 追従対象が画面外に出ないように補正
	camera_->translation_.x = max(camera_->translation_.x, objectivePoint_.x + targetMargin.left);
	camera_->translation_.x = min(camera_->translation_.x, objectivePoint_.x + targetMargin.right);
	camera_->translation_.y = max(camera_->translation_.y, objectivePoint_.y + targetMargin.bottom);
	camera_->translation_.y = min(camera_->translation_.y, objectivePoint_.y + targetMargin.top);

	// 移動範囲宣言
	camera_->translation_.x = max(camera_->translation_.x, movableArea_.left);
	camera_->translation_.x = min(camera_->translation_.x, movableArea_.right);
	camera_->translation_.y = max(camera_->translation_.y, movableArea_.bottom);
	camera_->translation_.y = min(camera_->translation_.y, movableArea_.top);

	// 行列を更新する
	camera_->UpdateMatrix();
}

// リセット
void CameraController::Reset() { 
	// 追従対象のワールドトランスフォーム
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;

}
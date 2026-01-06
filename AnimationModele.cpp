#include "AnimationModel.h"
#include <iostream>

using namespace KamataEngine;

void AnimatedModel::Load(const std::string& filename) {
	tinygltf::Model gltfModel;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool result = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, filename);

	if (!warn.empty()) {
		std::cout << "glTF warn: " << warn << std::endl;
	}
	if (!err.empty()) {
		std::cout << "glTF err: " << err << std::endl;
	}
	if (!result) {
		assert(false && "Failed to load glTF");
	}

	// ===== ここでアニメーション名を読み込む ===
	LoadAnimations(gltfModel);
}

int AnimatedModel::FindAnimation(const std::string& name) const {
	for (int i = 0; i < animations_.size(); i++) {
		if (animations_[i].name == name) {
			return i;
		}
	}
	return -1;
}

void AnimatedModel::PlayAnimation(int index, bool loop) {
	if (currentAnim_ == index) {
		return;
	}

	currentAnim_ = index;
	animTime_ = 0.0f;
	loop_ = loop;
}

void AnimatedModel::Update(float deltaTime) {
	if (currentAnim_ < 0) {
		return;
	}

	animTime_ += deltaTime;

	float duration = animations_[currentAnim_].duration;

	if (loop_) {
		if (animTime_ > duration) {
			animTime_ = fmod(animTime_, duration);
		}
	} else {
		if (animTime_ > duration) {
			animTime_ = duration;
		}
	}

	// TODO: ここでボーン行列計算（後で実装）
}

void AnimatedModel::Draw(const WorldTransform& worldTransform, const Camera& camera) { model_->Draw(worldTransform, camera); }
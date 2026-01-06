#pragma once
#pragma once
#include "KamataEngine.h"
#include <string>
#include <vector>

namespace KamataEngine {

struct Bone {
	Matrix4x4 offsetMatrix;
	int parentIndex;
};

struct AnimationClip {
	std::string name;
	float duration;
};

class AnimatedModel {
public:
	void Load(const std::string& filename);

	void PlayAnimation(int index, bool loop);
	void Update(float deltaTime);
	void Draw(const WorldTransform& worldTransform, const Camera& camera);

	int FindAnimation(const std::string& name) const;

private:
	std::unique_ptr<Model> model_; // 描画自体は既存Modelを利用

	std::vector<Bone> bones_;
	std::vector<Matrix4x4> boneMatrices_;

	std::vector<AnimationClip> animations_;

	int currentAnim_ = -1;
	float animTime_ = 0.0f;
	bool loop_ = true;
};

} // namespace KamataEngine
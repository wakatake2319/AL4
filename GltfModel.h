#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

class GltfModel {
public:
	bool Load(const std::string& path);

	const aiScene* GetScene() const { return scene_; }

private:
	Assimp::Importer importer_;
	const aiScene* scene_ = nullptr;
};
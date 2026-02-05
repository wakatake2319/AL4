#include "GltfModel.h"


bool GltfModel::Load(const std::string& path) {
	scene_ = importer_.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	return scene_ && scene_->HasMeshes();
}